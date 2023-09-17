#include "animation/types/AnimationReader132.h"

using namespace WarframeExporter::Animation;

// Lotsa attributes: /Lotus/Animations/LevelEvents/CmpMummy/InarosGolemGroundHitA 
// 3 Skeletons: /Lotus/Animations/Tenno/Melee/Whip/StealthHit_anim.fbx
void
AnimationReader132::readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
	uint32_t pathLen1 = headerReader->readUInt32();
	headerReader->seek(pathLen1, std::ios_base::cur);
	uint32_t pathLen2 = headerReader->readUInt32();
	headerReader->seek(pathLen2, std::ios_base::cur);

	headerReader->seek(0x2, std::ios_base::cur);

	// /Lotus/Animations/Tenno/Powers/MonkeyKing/GoldenBandStaff/ComboDefaultDWEP_anim.fbx
	// Import scale?
	headerReader->seek(0xC, std::ios_base::cur);

	headerReader->seek(0x46, std::ios_base::cur);
	static const std::string unkByteNum = "Unk Byte Test";
	headerReader->readUInt8(1, 3, unkByteNum);
	headerReader->seek(5, std::ios_base::cur);

	static const std::string skeletonCountMsg = "Skeleton Count";
	uint32_t skeletonCount = headerReader->readUInt32(0, 10, skeletonCountMsg);

	for (uint32_t x = 0; x < skeletonCount; x++)
	{
		uint32_t skeletonNameLen = headerReader->readUInt32();
		headerReader->seek(skeletonNameLen, std::ios_base::cur);

		// Looks like some important number
		uint32_t unkInt = headerReader->readUInt32();
		if (unkInt > 0)
			headerReader->seek(1, std::ios_base::cur);
		headerReader->seek(4, std::ios_base::cur);

		// Vector4
		headerReader->seek(0x20, std::ios_base::cur);

		static const std::string boneTreeCountMsg = "Bone Tree Count";
		uint32_t boneTreeCount = headerReader->readUInt32(0, 400, boneTreeCountMsg);
		static const std::string boneTreeNameLenMsg = "Bone Data name length";
		static const std::string boneTreeIndexMsg = "Bone Data index";
		for (uint32_t x = 0; x < boneTreeCount; x++)
		{
			uint32_t boneTreeNameLen = headerReader->readUInt32(0, 100, boneTreeNameLenMsg);
			headerReader->seek(boneTreeNameLen, std::ios_base::cur);
			headerReader->readUInt16(0, boneTreeCount + 1, boneTreeIndexMsg); // parent index
			headerReader->readUInt16();
		}
		
		const static std::string actionCountMsg = "Action Count";
		uint32_t actionCount = headerReader->readUInt32(0, 100, actionCountMsg);
		for (uint32_t x = 0; x < actionCount; x++)
		{
			headerReader->seek(0x10, std::ios_base::cur);
			uint32_t nameLen = headerReader->readUInt32();
			headerReader->seek(nameLen, std::ios_base::cur);
			headerReader->seek(0x4C, std::ios_base::cur);
		}
		
		headerReader->seek(0x1A, std::ios_base::cur);
		// FrameData
		headerReader->seek(0x4, std::ios_base::cur);
		for (uint32_t x = 0; x < actionCount; x++)
		{
			const static std::string boneConstantsMsg = "Actions bone data - should be same as bone count";
			delete[] headerReader->readUInt32Array(3, boneTreeCount, boneTreeCount + 1, boneConstantsMsg);
			// Unk Skip
			headerReader->seek(0x4, std::ios_base::cur);
		}
			headerReader->seek(0x8, std::ios_base::cur);
	}

	uint32_t errorCount = headerReader->readUInt32();
	for (uint32_t x = 0; x < errorCount; x++)
	{
		uint32_t errorCountStrLen = headerReader->readUInt32();
		headerReader->seek(errorCountStrLen, std::ios_base::cur);
	}

	if (headerReader->tell() != headerReader->getLength())
		throw unknown_format_error("Did not reach end of Header");
}

void
AnimationReader132::readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationHeaderExternal& outHeader)
{
	uint32_t pathLen1 = headerReader->readUInt32();
	headerReader->seek(pathLen1, std::ios_base::cur);
	uint32_t pathLen2 = headerReader->readUInt32();
	headerReader->seek(pathLen2, std::ios_base::cur);

	headerReader->seek(0x5A, std::ios_base::cur);

	uint32_t skeletonCount = headerReader->readUInt32();
	outHeader.skeletons.resize(skeletonCount);
	for (uint32_t x = 0; x < skeletonCount; x++)
	{
		AnimationSkeletonExternal& curSkel = outHeader.skeletons[x];

		uint32_t skeletonNameLen = headerReader->readUInt32();
		curSkel.name = headerReader->readAsciiString(skeletonNameLen);

		uint32_t unkInt = headerReader->readUInt32();
		if (unkInt > 0)
			headerReader->seek(1, std::ios_base::cur);
		headerReader->seek(4, std::ios_base::cur);

		headerReader->seek(0x20, std::ios_base::cur);

		uint32_t boneTreeCount = headerReader->readUInt32();
		curSkel.bones.resize(boneTreeCount);
		for (uint32_t x = 0; x < boneTreeCount; x++)
		{
			BoneTreeNodeExternal& curBone = curSkel.bones[x];

			uint32_t boneTreeNameLen = headerReader->readUInt32();
			curBone.name = headerReader->readAsciiString(boneTreeNameLen);
			curBone.parentIndex = headerReader->readUInt16();
			headerReader->seek(2, std::ios_base::cur);
		}
		curSkel.bones[0].parentIndex = -1;

		// Actions
		uint32_t actionCount = headerReader->readUInt32();
		curSkel.actions.resize(actionCount);
		for (uint32_t x = 0; x < actionCount; x++)
		{
			ActionHeaderExternal& curAction = curSkel.actions[x];

			headerReader->seek(0x10, std::ios_base::cur);
			uint32_t nameLen = headerReader->readUInt32();
			curAction.name = headerReader->readAsciiString(nameLen);
			headerReader->seek(0x44, std::ios_base::cur);
			curAction.frameStride = headerReader->readUInt32();
			curAction.frameOffset = headerReader->readUInt32();
		}

		headerReader->seek(0x1A, std::ios_base::cur);
		// Per-skeleton data
		curSkel.frameDataLen = headerReader->readUInt32();

		// More Action data
		for (uint32_t x = 0; x < actionCount; x++)
		{
			headerReader->seek(0xC, std::ios_base::cur);
			curSkel.actions[x].frameCount = headerReader->readUInt32();
		}
		headerReader->seek(0x8, std::ios_base::cur);
	}

	uint32_t errorCount = headerReader->readUInt32();
	for (uint32_t x = 0; x < errorCount; x++)
	{
		uint32_t errorCountStrLen = headerReader->readUInt32();
		headerReader->seek(errorCountStrLen, std::ios_base::cur);
	}

	if (headerReader->tell() != headerReader->getLength())
		throw unknown_format_error("Did not reach end of header");
}

void
AnimationReader132::isPureChannelType(uint16_t* channelTypes, int channelCount)
{
	std::unordered_map<uint16_t, int> channelTypeCounts;
	int correctedCount = channelCount;
	for (int x = 0; x < channelCount; x++)
	{
		if (channelTypes[x] != 0)
		{
			if (channelTypes[x] == 2)
				channelTypeCounts[channelTypes[x]]++;
		}
		else
			correctedCount--;
	}
	for (const auto& x : channelTypeCounts)
	{
		if (x.second / correctedCount > .00 && channelTypes[0] != 0)
			throw std::runtime_error("Pure channel type " + std::to_string(x.first) + " (" + std::to_string(x.second) + "/" + std::to_string(correctedCount) + ")");
	}
}

void
AnimationReader132::readBodyDebug(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
	// Strange data
	// /Lotus/Animations/Cinematics/TheNewWar/KahlIntroCinDebris/SkiffCrashRockDebris_anim.fbx
	for (uint32_t x = 0; x < extHeader.skeletons.size(); x++)
	{
		const AnimationSkeletonExternal& curSkel = extHeader.skeletons[x];

		bodyReader->seek(curSkel.frameDataLen, std::ios_base::cur); // Frame Data

		for (uint32_t y = 0; y < curSkel.actions.size(); y++)
		{
			const ActionHeaderExternal& curAction = curSkel.actions[y];

			// These values can be huge, no point in checking
			bodyReader->seek(curSkel.bones.size() * 10 * 4, std::ios_base::cur);

			const std::string channelCountMsg = "Channel Count";
			uint16_t* channelCounts = bodyReader->readUInt16Array(curSkel.bones.size(), 0, 33, channelCountMsg);

			const static std::string strideMsg = "Current Frame Data Offsets";
			uint16_t* offsets = bodyReader->readUInt16Array(curSkel.bones.size(), 0, curAction.frameStride + 1, strideMsg);

			const static std::string timescaleMsg = "Timescale";
			delete[] bodyReader->readFloatArray(curAction.frameCount, 0.0F, 1.00001F, CONV_ZERO | FAIL_SUBNORM, timescaleMsg);

			/*
			* Pure channel type 1 (9/9): /Lotus/Animations/FlightSuit/UnEquipSpaceSwordWEP
			* Pure channel type 1 (15/15): /Lotus/Fx/PowersuitAbilities/Paladin/ReckoningAvatarAttach_anim.fbx
			* Pure channel type 1 (5/5): /Lotus/Objects/Orokin/BaseSet/Props/ORKxObelisk/ORKxObeliskRepellent3Activated_anim.fbx
			* Pure channel type 2 (4/4): /Lotus/Characters/Tenno/Caliban/CalibanFinsLLegSway_anim.fbx
			* Pure channel type 2 (5/5): /Lotus/Objects/Orokin/BaseSet/Props/ORKxObelisk/ORKxObeliskRepellent2Idle_anim.fbx
			* Pure channel type 3 (5/5): /Lotus/Objects/Orokin/BaseSet/Props/ORKxObelisk/ORKxObeliskRepellent5Collapsed_anim.fbx
			* Pure channel type 3 (14/14): /Lotus/Animations/LevelEvents/GasCityRemastered/RopyBossArenaPlatformCollapse/Phase3_anim.fbx
			* Pure channel type 3 (37/37): /Lotus/Animations/Infested/Golem/MeleeEmergeAndSwingLR_anim.fbx
			* Pure channel type 8 (2/2): /Lotus/Animations/Tenno/Menu/Idles/Infestation/Ailerons/NeckClosedToOpen_anim.fbx
			* Pure channel type 8 (1/1): /Lotus/Characters/Tenno/Priest/HarrowDeluxe/HarrowDeluxeHeart_anim.fbx
			* Pure channel type 9 (1/1): /Lotus/Characters/Tenno/Antimatter/NovaDeluxeII/Canister_anim.fbx
			* Pure channel type 9 (1/1): /Lotus/Characters/Tenno/Engineer/EngineerDeluxeValveAmbient_anim.fbx
			* Pure channel type 10 (4/4): /Lotus/Characters/Tenno/Accessory/Scarves/GarudaCape/GarudaCapeBonesIdle_anim.fbx
			* Pure channel type 11 (2/2): /Lotus/Weapons/Tenno/Melee/Tonfa/MagDeluxeTonfa/MagDeluxeTonfaEquip_anim.fbx
			* Pure channel type 11 (5/5): /Lotus/Animations/Creatures/InfestationPod/HoldIdleA_anim.fbx
			* Pure channel type 11 (19/19): /Lotus/Fx/Levels/SentientDevourer/RegrowthCatalysingPlatformB_anim.fbx
			*/
			for (size_t z = 0; z < curSkel.actions[y].frameCount; z++)
			{
				for (size_t w = 0; w < curSkel.bones.size() - 1; w++)
				{
					uint16_t curChannelType = channelCounts[w];
					uint16_t off1 = offsets[w + 1];
					uint16_t off0 = offsets[w];
					uint16_t curChannelDataLen = offsets[w + 1] - offsets[w];

					switch (curChannelType)
					{
					case 0:
					case 32:
						if (curChannelDataLen != 0)
							throw unknown_format_error("Channel type 32 data len != 0");
						break;
					case 4:
						if (curChannelDataLen != 4)
							throw unknown_format_error("Channel type 4 data len != 4");
						break;
					case 1:
					case 2:
					case 8:
						if (curChannelDataLen != 6)
							throw unknown_format_error("Channel type 1/2/8 data len != 6");
						break;
					case 3:
					case 9:
					case 10:
						if (curChannelDataLen != 12)
							throw unknown_format_error("Channel type 3/9/10 data len != 12");
						break;
					case 11:
						if (curChannelDataLen != 18)
							throw unknown_format_error("Channel type 11 data len != 18");
						break;
					default:
						throw unknown_format_error("No case for channel type " + std::to_string(curChannelType) + " (data len = " + std::to_string(curChannelDataLen) + ")");
					}
				}
			}
			isPureChannelType(channelCounts, curSkel.bones.size());

			delete[] channelCounts;
			delete[] offsets;
		}
	}

	if (bodyReader->tell() != bodyReader->getLength())
		throw unknown_format_error("Did not reach end of body");
}

void
AnimationReader132::readBody(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationBodyExternal& outBody)
{
	outBody.actions.resize(extHeader.skeletons.size());
	for (uint32_t x = 0; x < extHeader.skeletons.size(); x++)
	{
		const AnimationSkeletonExternal& curSkel = extHeader.skeletons[x];
		outBody.actions[x] = std::vector<ActionBodyExternal>(curSkel.actions.size());

		// Frame data, will come back later
		size_t frameDataPos = bodyReader->tell();
		bodyReader->seek(curSkel.frameDataLen, std::ios_base::cur);

		for (uint32_t y = 0; y < curSkel.actions.size(); y++)
		{
			const ActionHeaderExternal& curActionHead = curSkel.actions[y];
			ActionBodyExternal& curActionBody = outBody.actions[x][y];

			curActionBody.initialTransform.pos.resize(curSkel.bones.size());
			curActionBody.initialTransform.rot.resize(curSkel.bones.size());
			curActionBody.initialTransform.scale.resize(curSkel.bones.size());
			for (size_t x = 0; x < curSkel.bones.size(); x++)
			{
				BoneTransform& initTransform = curActionBody.initialTransform;

				bodyReader->readFloatArray(&initTransform.rot[x].w, 4);
				bodyReader->readFloatArray(&initTransform.pos[x].x, 3);
				bodyReader->readFloatArray(&initTransform.scale[x].x, 3);
			}

			uint16_t* channelCounts = bodyReader->readUInt16Array(curSkel.bones.size());

			// Channel Strides
			bodyReader->seek(curSkel.bones.size() * 2, std::ios_base::cur);

			std::vector<float> timeScales(curActionHead.frameCount);
			bodyReader->readFloatArray(timeScales.data(), curActionHead.frameCount);
			curActionBody.timeScale = timeScales;

			// Back to frame data
			size_t actionEndPos = bodyReader->tell();
			bodyReader->seek(frameDataPos, std::ios_base::beg);

			curActionBody.transforms.resize(curSkel.bones.size());
			for (size_t z = 0; z < curActionHead.frameCount; z++)
			{
				for (size_t w = 0; w < curSkel.bones.size(); w++)
				{
					uint16_t curChannelType = channelCounts[w];
					BoneTransform& curTransform = curActionBody.transforms[w];

					// I think this can be re-written as binary flags.
					switch (curChannelType)
					{
					case 0:
					case 32:
						//bodyReader->seek(0, std::ios_base::cur);
						break;
					case 1:
					{
						glm::vec3 vec = {
							bodyReader->readInt16() / 32767.0F,
							bodyReader->readInt16() / 32767.0F * -1,
							bodyReader->readInt16() / 32767.0F,
						};
						curTransform.pos.push_back(vec);
						break;
					}
					case 2:
					{
						uint16_t r_a = bodyReader->readUInt16();
						uint16_t r_b = bodyReader->readUInt16();
						uint16_t r_c = bodyReader->readUInt16();
						glm::quat unpacked = unpackQuaternion(r_a, r_b, r_c, false);

						curTransform.rot.push_back(unpacked);

						break;
					}
					// MeleeEmergeAndSwingLR_anim
					case 3:
					{
						// Position
						glm::vec3 vec = {
							bodyReader->readInt16() / 32767.0F,
							bodyReader->readInt16() / 32767.0F * -1,
							bodyReader->readInt16() / 32767.0F,
						};
						curTransform.pos.push_back(vec);

						// Rotation
						uint16_t r_a = bodyReader->readUInt16();
						uint16_t r_b = bodyReader->readUInt16();
						uint16_t r_c = bodyReader->readUInt16();
						glm::quat unpacked = unpackQuaternion(r_a, r_b, r_c, true);

						curTransform.rot.push_back(unpacked);

						break;
					}
					case 4:
					{
						bodyReader->seek(4, std::ios_base::cur);
						break;
					}
					case 8:
					{
						// 3 half-floats
						bodyReader->seek(6, std::ios_base::cur);
						break;
					}
					case 9:
					{
						// 6 shorts (divide by SHORT_MAX)
						// Lots of 0s. Scale/translation?
						bodyReader->seek(12, std::ios_base::cur);
						break;
					}
					case 10:
					{
						// 3 single-precision floats
						bodyReader->seek(12, std::ios_base::cur);
						break;
					}
					case 11:
					{
						// 3 shorts (divide by SHORT_MAX), 1 float, 1 ushort, 1 ushort, 1 float
						bodyReader->seek(18, std::ios_base::cur);
						break;
					}
					default:
						throw unknown_format_error("No case for channel count " + std::to_string(curChannelType));
					}
				}
			}
			bodyReader->seek(actionEndPos, std::ios_base::beg);
			delete[] channelCounts;
		}
	}

	if (bodyReader->tell() != bodyReader->getLength())
		throw unknown_format_error("Did not reach end of body");
}

glm::quat
AnimationReader132::unpackQuaternion(const uint16_t& rawA, const uint16_t& rawB, const uint16_t rawC, bool flipSigns)
{
	float cq_Sqrt2 = 1.4142135623730950488016887242097F;
	float int2Float = 1.0f / (16384.f * cq_Sqrt2);
	const int Mapping[4][3] = { {1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2} };


	// Move D, E, G, H, J into sequential order
	uint16_t totalFlag = (rawC & 0x3) | ((rawB >> 12) & 0xC) | ((rawA << 4) & 0x10) | ((rawA >> 10) & 0x20);

	// Split up totalFlag
	std::array<int16_t, 4> signBits;
	signBits[0] = (totalFlag >> 5) & 1;
	signBits[1] = (totalFlag >> 4) & 1;
	signBits[2] = (totalFlag >> 3) & 1;
	signBits[3] = (totalFlag >> 2) & 1;
	uint16_t largest = totalFlag & 3;

	const int* map = Mapping[largest];

	// If these integers are signed...
	//  - Create signed bit
	//  - Fill in missing bit with 1 (15bit -> 16bit conversion)
	int16_t c_a = (rawA >> 1) & 0x3FFF;
	if (!signBits[0])
		c_a |= 0xC000;

	int16_t c_b = rawB & 0x3FFF;
	if (!signBits[3])
		c_b |= 0xC000;

	int16_t c_c = (rawC >> 2) & 0x3FFF;
	if (!signBits[1])
		c_c |= 0xC000;

	glm::quat rot;
	rot[map[0]] = signBits[3] > 0 && flipSigns ? -(c_b * int2Float) : (c_b * int2Float);
	rot[map[1]] = signBits[0] > 0 && flipSigns ? -(c_a * int2Float) : (c_a * int2Float);
	rot[map[2]] = signBits[1] > 0 && flipSigns ? -(c_c * int2Float) : (c_c * int2Float);

	float largestNum = sqrt(std::max(1.0f - rot[map[0]] * rot[map[0]] - rot[map[1]] * rot[map[1]] - rot[map[2]] * rot[map[2]], 0.0f));
	rot[largest] = signBits[2] > 0 ? largestNum : -largestNum;

	return std::move(rot);
}