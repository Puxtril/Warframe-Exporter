#pragma once

/*
Examples
174676543101364914
1:
	/Lotus/Interface/Icons/Objects/ExplosiveBarrel.png
	/Lotus/Interface/Icons/Objects/SecurityCamera.png
2:
	/Lotus/Interface/Icons/InvasionCorpus_d.png
	/Lotus/Interface/Icons/InvasionGrineer_d.png
	/Lotus/Interface/Icons/InvasionInfested_d.png
3:
	/Lotus/Characters/Tenno/Excalibur/DiscordSkin/ExcaliburDiscordSkinPackmap
	/Lotus/Characters/Tenno/Excalibur/DiscordSkin/ExcaliburHelmetDiscordSkinPackmap
	/Lotus/Interface/Icons/Infinity_d.png
6:
	/Lotus/Interface/Backgrounds/Helminth/BgFull.png
	/Lotus/Interface/Backgrounds/Fortuna/FortunaBackground.png
	/Lotus/Levels/CivilianHubs/VenusLandscape/Landscape0/HolesMaps/T7u6_lxm.png
7:
	/Lotus/Levels/CivilianHubs/VenusLandscape/Landscape0/MacronormalMaps/T2u2_lnm.png
	/Lotus/Interface/Icons/Symbols/GenericOrokinSymbols10_p.png
10:
	/Lotus/Interface/Icons/MarketCheckmark.png
	/Lotus/Interface/Quests/OrokinLockIconBacker_d.png
34:
35:
*/
namespace WarframeExporter
{
	namespace Texture
	{
		enum class TextureFormatExternal
		{
			Default = 0,
			BC1 = 1,
			BC2 = 2,
			BC3 = 3,
			BC4 = 6,
			BC5 = 7,
			Uncompressed = 10,
			BC7 = 34,
			BC6 = 35,
		};
	}
}