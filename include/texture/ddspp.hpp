// Sources
// https://docs.microsoft.com/en-us/windows/uwp/gaming/complete-code-for-ddstextureloader
// https://docs.microsoft.com/en-us/windows/desktop/direct3ddds/dds-header
// https://docs.microsoft.com/en-us/windows/desktop/direct3ddds/dds-header-dxt10

#pragma once

#include <fstream>

namespace WarframeExporter
{
	#if (__cpp_constexpr == 201304) || (_MSC_VER > 1900)
	#define ddspp_constexpr constexpr
	#else
	#define ddspp_constexpr const
	#endif

	namespace ddspp
	{
		namespace internal
		{
			// PixelFormat Flag Constants
			static ddspp_constexpr unsigned int DDS_FOURCC     = 0x00000004; // DDPF_FOURCC
			static ddspp_constexpr unsigned int DDS_RGB        = 0x00000040; // DDPF_RGB
			static ddspp_constexpr unsigned int DDS_RGBA       = 0x00000041; // DDPF_RGB | DDPF_ALPHAPIXELS
			static ddspp_constexpr unsigned int DDS_LUMINANCE  = 0x00020000; // DDPF_LUMINANCE
			static ddspp_constexpr unsigned int DDS_LUMINANCEA = 0x00020001; // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
			static ddspp_constexpr unsigned int DDS_ALPHA      = 0x00000002; // DDPF_ALPHA
			static ddspp_constexpr unsigned int DDS_PAL8       = 0x00000020; // DDPF_PALETTEINDEXED8
			static ddspp_constexpr unsigned int DDS_PAL8A      = 0x00000021; // DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS
			static ddspp_constexpr unsigned int DDS_BUMPDUDV   = 0x00080000; // DDPF_BUMPDUDV

			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_CAPS        = 0x00000001; // DDSD_CAPS
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_HEIGHT      = 0x00000002; // DDSD_HEIGHT
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_WIDTH       = 0x00000004; // DDSD_WIDTH
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_PITCH       = 0x00000008; // DDSD_PITCH
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_PIXELFORMAT = 0x00001000; // DDSD_PIXELFORMAT
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_MIPMAP      = 0x00020000; // DDSD_MIPMAPCOUNT
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_LINEARSIZE  = 0x00080000; // DDSD_LINEARSIZE
			static ddspp_constexpr unsigned int DDS_HEADER_FLAGS_VOLUME      = 0x00800000; // DDSD_DEPTH

			static ddspp_constexpr unsigned int DDS_HEADER_CAPS_COMPLEX      = 0x00000008; // DDSCAPS_COMPLEX
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS_MIPMAP       = 0x00400000; // DDSCAPS_MIPMAP
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS_TEXTURE      = 0x00001000; // DDSCAPS_TEXTURE

			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP           = 0x00000200; // DDSCAPS2_CUBEMAP
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_POSITIVEX = 0x00000600; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEX = 0x00000a00; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_POSITIVEY = 0x00001200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEY = 0x00002200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_POSITIVEZ = 0x00004200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEZ = 0x00008200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_VOLUME            = 0x00200000; // DDSCAPS2_VOLUME
			static ddspp_constexpr unsigned int DDS_HEADER_CAPS2_CUBEMAP_ALLFACES  = 
				DDS_HEADER_CAPS2_CUBEMAP_POSITIVEX | DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEX | DDS_HEADER_CAPS2_CUBEMAP_POSITIVEY | 
				DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEY | DDS_HEADER_CAPS2_CUBEMAP_POSITIVEZ | DDS_HEADER_CAPS2_CUBEMAP_NEGATIVEZ;

			static ddspp_constexpr unsigned int DXGI_MISC_FLAG_CUBEMAP = 0x2; // https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_resource_misc_flag
			static ddspp_constexpr unsigned int DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7;

			ddspp_constexpr inline unsigned int MakeFourCC(const char* value) { return value[0] + (value[1] << 8) + (value[2] << 16) + (value[3] << 24); }

			struct PixelFormat
			{
				unsigned int size;
				unsigned int flags;
				unsigned int fourCC;
				unsigned int RGBBitCount;
				unsigned int RBitMask;
				unsigned int GBitMask;
				unsigned int BBitMask;
				unsigned int ABitMask;
			};
			static ddspp_constexpr unsigned int PFSize = sizeof(PixelFormat);
			static_assert(PFSize == 32, "PixelFormat size mismatch");
		
			static ddspp_constexpr PixelFormat PF_DXT1 		= {PFSize, DDS_FOURCC, MakeFourCC("DXT1"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_DXT2 		= {PFSize, DDS_FOURCC, MakeFourCC("DXT2"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_DXT3 		= {PFSize, DDS_FOURCC, MakeFourCC("DXT3"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_DXT4		= {PFSize, DDS_FOURCC, MakeFourCC("DXT4"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_DXT5 		= {PFSize, DDS_FOURCC, MakeFourCC("DXT5"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_BC4UNORM	= {PFSize, DDS_FOURCC, MakeFourCC("ATI1"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_BC4SNORM	= {PFSize, DDS_FOURCC, MakeFourCC("BC4S"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_BC5UNORM	= {PFSize, DDS_FOURCC, MakeFourCC("ATI2"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_BC5SNORM	= {PFSize, DDS_FOURCC, MakeFourCC("BC5S"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_R8G8B8G8	= {PFSize, DDS_FOURCC, MakeFourCC("RGBG"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_G8R8G8B8	= {PFSize, DDS_FOURCC, MakeFourCC("GRGB"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_YUY2 		= {PFSize, DDS_FOURCC, MakeFourCC("YUY2"), 0, 0, 0, 0, 0};
			static ddspp_constexpr PixelFormat PF_A8R8G8B8	= {PFSize, DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};
			static ddspp_constexpr PixelFormat PF_X8R8G8B8	= {PFSize, DDS_RGB, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000};
			static ddspp_constexpr PixelFormat PF_A8B8G8R8	= {PFSize, DDS_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
			static ddspp_constexpr PixelFormat PF_X8B8G8R8	= {PFSize, DDS_RGB, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000};
			static ddspp_constexpr PixelFormat PF_G16R16 	= {PFSize, DDS_RGB, 0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000};
			static ddspp_constexpr PixelFormat PF_R5G6B5 	= {PFSize, DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000};
			static ddspp_constexpr PixelFormat PF_A1R5G5B5	= {PFSize, DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000};
			static ddspp_constexpr PixelFormat PF_A4R4G4B4	= {PFSize, DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000};
			static ddspp_constexpr PixelFormat PF_R8G8B8 	= {PFSize, DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000};
			static ddspp_constexpr PixelFormat PF_L8 		= {PFSize, DDS_LUMINANCE, 0, 8, 0xff, 0x00, 0x00, 0x00};
			static ddspp_constexpr PixelFormat PF_L16 		= {PFSize, DDS_LUMINANCE, 0, 16, 0xffff, 0x0000, 0x0000, 0x0000};
			static ddspp_constexpr PixelFormat PF_A8L8 		= {PFSize, DDS_LUMINANCEA, 0, 16, 0x00ff, 0x0000, 0x0000, 0xff00};
			static ddspp_constexpr PixelFormat PF_A8L8ALT 	= {PFSize, DDS_LUMINANCEA, 0, 8, 0x00ff, 0x0000, 0x0000, 0xff00};
			static ddspp_constexpr PixelFormat PF_A8 		= {PFSize, DDS_ALPHA, 0, 8, 0x00, 0x00, 0x00, 0xff};
			static ddspp_constexpr PixelFormat PF_V8U8 		= {PFSize, DDS_BUMPDUDV, 0, 16, 0x00ff, 0xff00, 0x0000, 0x0000};
			static ddspp_constexpr PixelFormat PF_Q8W8V8U8	= {PFSize, DDS_BUMPDUDV, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
			static ddspp_constexpr PixelFormat PF_V16U16 	= {PFSize, DDS_BUMPDUDV, 0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000};
			static ddspp_constexpr PixelFormat PF_DX10 		= {PFSize, DDS_FOURCC, MakeFourCC("DX10"), 0, 0, 0, 0, 0};


			inline ddspp_constexpr bool is_rgba_mask(const PixelFormat& ddspf, unsigned int rmask, unsigned int gmask, unsigned int bmask, unsigned int amask)
			{
				return (ddspf.RBitMask == rmask) && (ddspf.GBitMask == gmask) && (ddspf.BBitMask == bmask) && (ddspf.ABitMask == amask);
			}

			inline ddspp_constexpr bool is_rgb_mask(const PixelFormat& ddspf, unsigned int rmask, unsigned int gmask, unsigned int bmask)
			{
				return (ddspf.RBitMask == rmask) && (ddspf.GBitMask == gmask) && (ddspf.BBitMask == bmask);
			}
		}

		using namespace internal;

		// https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_resource_dimension
		enum DXGIResourceDimension : unsigned char
		{
			DXGI_Unknown,
			DXGI_Buffer,
			DXGI_Texture1D,
			DXGI_Texture2D,
			DXGI_Texture3D
		};

		// Matches DXGI_FORMAT https://docs.microsoft.com/en-us/windows/desktop/api/dxgiformat/ne-dxgiformat-dxgi_format
		enum DXGIFormat : unsigned int
		{
			UNKNOWN	                    = 0,
			R32G32B32A32_TYPELESS       = 1,
			R32G32B32A32_FLOAT          = 2,
			R32G32B32A32_UINT           = 3,
			R32G32B32A32_SINT           = 4,
			R32G32B32_TYPELESS          = 5,
			R32G32B32_FLOAT             = 6,
			R32G32B32_UINT              = 7,
			R32G32B32_SINT              = 8,
			R16G16B16A16_TYPELESS       = 9,
			R16G16B16A16_FLOAT          = 10,
			R16G16B16A16_UNORM          = 11,
			R16G16B16A16_UINT           = 12,
			R16G16B16A16_SNORM          = 13,
			R16G16B16A16_SINT           = 14,
			R32G32_TYPELESS             = 15,
			R32G32_FLOAT                = 16,
			R32G32_UINT                 = 17,
			R32G32_SINT                 = 18,
			R32G8X24_TYPELESS           = 19,
			D32_FLOAT_S8X24_UINT        = 20,
			R32_FLOAT_X8X24_TYPELESS    = 21,
			X32_TYPELESS_G8X24_UINT     = 22,
			R10G10B10A2_TYPELESS        = 23,
			R10G10B10A2_UNORM           = 24,
			R10G10B10A2_UINT            = 25,
			R11G11B10_FLOAT             = 26,
			R8G8B8A8_TYPELESS           = 27,
			R8G8B8A8_UNORM              = 28,
			R8G8B8A8_UNORM_SRGB         = 29,
			R8G8B8A8_UINT               = 30,
			R8G8B8A8_SNORM              = 31,
			R8G8B8A8_SINT               = 32,
			R16G16_TYPELESS             = 33,
			R16G16_FLOAT                = 34,
			R16G16_UNORM                = 35,
			R16G16_UINT                 = 36,
			R16G16_SNORM                = 37,
			R16G16_SINT                 = 38,
			R32_TYPELESS                = 39,
			D32_FLOAT                   = 40,
			R32_FLOAT                   = 41,
			R32_UINT                    = 42,
			R32_SINT                    = 43,
			R24G8_TYPELESS              = 44,
			D24_UNORM_S8_UINT           = 45,
			R24_UNORM_X8_TYPELESS       = 46,
			X24_TYPELESS_G8_UINT        = 47,
			R8G8_TYPELESS               = 48,
			R8G8_UNORM                  = 49,
			R8G8_UINT                   = 50,
			R8G8_SNORM                  = 51,
			R8G8_SINT                   = 52,
			R16_TYPELESS                = 53,
			R16_FLOAT                   = 54,
			D16_UNORM                   = 55,
			R16_UNORM                   = 56,
			R16_UINT                    = 57,
			R16_SNORM                   = 58,
			R16_SINT                    = 59,
			R8_TYPELESS                 = 60,
			R8_UNORM                    = 61,
			R8_UINT                     = 62,
			R8_SNORM                    = 63,
			R8_SINT                     = 64,
			A8_UNORM                    = 65,
			R1_UNORM                    = 66,
			R9G9B9E5_SHAREDEXP          = 67,
			R8G8B8G8_UNORM              = 68,
			G8R8G8B8_UNORM              = 69,
			BC1_TYPELESS                = 70,
			BC1_UNORM                   = 71,
			BC1_UNORM_SRGB              = 72,
			BC2_TYPELESS                = 73,
			BC2_UNORM                   = 74,
			BC2_UNORM_SRGB              = 75,
			BC3_TYPELESS                = 76,
			BC3_UNORM                   = 77,
			BC3_UNORM_SRGB              = 78,
			BC4_TYPELESS                = 79,
			BC4_UNORM                   = 80,
			BC4_SNORM                   = 81,
			BC5_TYPELESS                = 82,
			BC5_UNORM                   = 83,
			BC5_SNORM                   = 84,
			B5G6R5_UNORM                = 85,
			B5G5R5A1_UNORM              = 86,
			B8G8R8A8_UNORM              = 87,
			B8G8R8X8_UNORM              = 88,
			R10G10B10_XR_BIAS_A2_UNORM  = 89,
			B8G8R8A8_TYPELESS           = 90,
			B8G8R8A8_UNORM_SRGB         = 91,
			B8G8R8X8_TYPELESS           = 92,
			B8G8R8X8_UNORM_SRGB         = 93,
			BC6H_TYPELESS               = 94,
			BC6H_UF16                   = 95,
			BC6H_SF16                   = 96,
			BC7_TYPELESS                = 97,
			BC7_UNORM                   = 98,
			BC7_UNORM_SRGB              = 99,
			AYUV                        = 100,
			Y410                        = 101,
			Y416                        = 102,
			NV12                        = 103,
			P010                        = 104,
			P016                        = 105,
			OPAQUE_420                  = 106,
			YUY2                        = 107,
			Y210                        = 108,
			Y216                        = 109,
			NV11                        = 110,
			AI44                        = 111,
			IA44                        = 112,
			P8                          = 113,
			A8P8                        = 114,
			B4G4R4A4_UNORM              = 115,
		};

		struct Header
		{
			unsigned int size;
			unsigned int flags;
			unsigned int height;
			unsigned int width;
			unsigned int pitchOrLinearSize;
			unsigned int depth;
			unsigned int mipMapCount;
			unsigned int reserved1[11];
			PixelFormat ddspf;
			unsigned int caps;
			unsigned int caps2;
			unsigned int caps3;
			unsigned int caps4;
			unsigned int reserved2;
		};

		static_assert(sizeof(Header) == 124, "DDS Header size mismatch");

		struct HeaderDXT10
		{
			DXGIFormat dxgiFormat;
			DXGIResourceDimension resourceDimension;
			unsigned int miscFlag;
			unsigned int arraySize;
			unsigned int reserved;
		};

		static_assert(sizeof(HeaderDXT10) == 20, "DDS DX10 Extended Header size mismatch");

		enum Result : unsigned char
		{
			Success,
			Error
		};

		enum TextureType : unsigned char
		{
			Texture1D,
			Texture2D,
			Texture3D,
			Cubemap,
		};

		struct Descriptor
		{
			DXGIFormat format;
			TextureType type;
			unsigned int width;
			unsigned int height;
			unsigned int depth;
			unsigned int numMips;
			unsigned int arraySize;
			unsigned int rowPitch; // Row pitch for mip 0
			unsigned int depthPitch; // Size of mip 0
			unsigned int bitsPerPixelOrBlock; // If compressed bits per block, else bits per pixel
			unsigned int blockWidth;
			unsigned int blockHeight;
			bool compressed;
			bool srgb;
			unsigned int headerSize; // Actual size of header, use this to get to image data
		};

		inline ddspp_constexpr bool is_compressed(DXGIFormat format)
		{
			return (format >= BC1_UNORM && format <= BC5_SNORM) || 
					(format >= BC6H_TYPELESS && format <= BC7_UNORM_SRGB);
		}

		inline ddspp_constexpr PixelFormat get_pixelformat(DXGIFormat format)
		{
			switch (format)
			{
			case R8G8B8A8_UNORM:
				return PF_A8B8G8R8;
			case R16G16_UNORM:
				return PF_G16R16;
			case R8G8_UNORM:
				return PF_A8L8;
			case R16_UNORM:
				return PF_L16;
			case R8_UNORM:
				return PF_L8;
			case A8_UNORM:
				return PF_A8;
			case R8G8B8G8_UNORM:
				return PF_R8G8B8G8;
			case G8R8G8B8_UNORM:
				return PF_G8R8G8B8;
			case BC1_UNORM:
				return PF_DXT1;
			case BC2_UNORM:
				return PF_DXT3;
			case BC3_UNORM:
				return PF_DXT5;
			case BC4_UNORM:
				return PF_BC4UNORM;
			case BC4_SNORM:
				return PF_BC4SNORM;
			case BC5_UNORM:
				return PF_BC5UNORM;
			case BC5_SNORM:
				return PF_BC5SNORM;
			case B5G6R5_UNORM:
				return PF_R5G6B5;
			case B5G5R5A1_UNORM:
				return PF_A1R5G5B5;
			case R8G8_SNORM:
				return PF_V8U8;
			case R8G8B8A8_SNORM:
				return PF_Q8W8V8U8;
			case R16G16_SNORM:
				return PF_V16U16;
			case B8G8R8A8_UNORM:
				return PF_A8R8G8B8;
			case B8G8R8X8_UNORM:
				return PF_X8R8G8B8;
			case B4G4R4A4_UNORM:
				return PF_A4R4G4B4;
			case YUY2:
				return PF_YUY2;
			default:
				return PF_DX10;
			}
		}

		inline ddspp_constexpr bool is_srgb(DXGIFormat format)
		{
			switch(format)
			{
				case R8G8B8A8_UNORM_SRGB:
				case BC1_UNORM_SRGB:
				case BC2_UNORM_SRGB:
				case BC3_UNORM_SRGB:
				case B8G8R8A8_UNORM_SRGB:
				case B8G8R8X8_UNORM_SRGB:
				case BC7_UNORM_SRGB:
					return true;
				default:
					return false;
			}
		}

		inline ddspp_constexpr unsigned int get_bits_per_pixel_or_block(DXGIFormat format)
		{
			switch(format)
			{
				case R1_UNORM:
					return 1;
				case R8_TYPELESS:
				case R8_UNORM:
				case R8_UINT:
				case R8_SNORM:
				case R8_SINT:
				case A8_UNORM:
				case AI44:
				case IA44:
				case P8:
					return 8;
				case NV12:
				case OPAQUE_420:
				case NV11:
					return 12;
				case R8G8_TYPELESS:
				case R8G8_UNORM:
				case R8G8_UINT:
				case R8G8_SNORM:
				case R8G8_SINT:
				case R16_TYPELESS:
				case R16_FLOAT:
				case D16_UNORM:
				case R16_UNORM:
				case R16_UINT:
				case R16_SNORM:
				case R16_SINT:
				case B5G6R5_UNORM:
				case B5G5R5A1_UNORM:
				case A8P8:
				case B4G4R4A4_UNORM:
					return 16;
				case P010:
				case P016:
					return 24;
				case BC1_UNORM:
				case BC1_UNORM_SRGB:
				case BC1_TYPELESS:
				case BC4_UNORM:
				case BC4_SNORM:
				case BC4_TYPELESS:
				case R16G16B16A16_TYPELESS:
				case R16G16B16A16_FLOAT:
				case R16G16B16A16_UNORM:
				case R16G16B16A16_UINT:
				case R16G16B16A16_SNORM:
				case R16G16B16A16_SINT:
				case R32G32_TYPELESS:
				case R32G32_FLOAT:
				case R32G32_UINT:
				case R32G32_SINT:
				case R32G8X24_TYPELESS:
				case D32_FLOAT_S8X24_UINT:
				case R32_FLOAT_X8X24_TYPELESS:
				case X32_TYPELESS_G8X24_UINT:
				case Y416:
				case Y210:
				case Y216:
					return 64;
				case R32G32B32_TYPELESS:
				case R32G32B32_FLOAT:
				case R32G32B32_UINT:
				case R32G32B32_SINT:
					return 96;
				case BC2_UNORM:
				case BC2_UNORM_SRGB:
				case BC2_TYPELESS:
				case BC3_UNORM:
				case BC3_UNORM_SRGB:
				case BC3_TYPELESS:
				case BC5_UNORM:
				case BC5_SNORM:
				case BC6H_UF16:
				case BC6H_SF16:
				case BC7_UNORM:
				case BC7_UNORM_SRGB:
				case R32G32B32A32_TYPELESS:
				case R32G32B32A32_FLOAT:
				case R32G32B32A32_UINT:
				case R32G32B32A32_SINT:
					return 128;
				default:
					return 32; // Most formats are 32 bits per pixel
					break;
			}
		}

		inline ddspp_constexpr void get_block_size(DXGIFormat format, unsigned int& blockWidth, unsigned int& blockHeight)
		{
			switch(format)
			{
				case BC1_UNORM:
				case BC1_UNORM_SRGB:
				case BC1_TYPELESS:
				case BC4_UNORM:
				case BC4_SNORM:
				case BC4_TYPELESS:
				case BC2_UNORM:
				case BC2_UNORM_SRGB:
				case BC2_TYPELESS:
				case BC3_UNORM:
				case BC3_UNORM_SRGB:
				case BC3_TYPELESS:
				case BC5_UNORM:
				case BC5_SNORM:
				case BC6H_UF16:
				case BC6H_SF16:
				case BC7_UNORM:
				case BC7_UNORM_SRGB:
					blockWidth = 4; blockHeight = 4;
					break;
				default:
					blockWidth = 1; blockHeight = 1;
					break;
			}

			return;
		}

		// Returns if a DXT10 header is used
		inline bool encode_header(const DXGIFormat format, const unsigned int width, const unsigned int height, const unsigned int depth,
									const TextureType type, const unsigned int mipCount, const unsigned int arraySize, 
									Header& header, HeaderDXT10& dxt10Header)
		{
			header.size = sizeof(Header);

			// Fill in header flags
			header.flags = DDS_HEADER_FLAGS_CAPS | DDS_HEADER_FLAGS_HEIGHT | DDS_HEADER_FLAGS_WIDTH | DDS_HEADER_FLAGS_PIXELFORMAT;
			header.caps = DDS_HEADER_CAPS_TEXTURE;
			header.caps2 = 0;

			if (mipCount > 1)
			{
				header.flags |= DDS_HEADER_FLAGS_MIPMAP;
				header.caps |= DDS_HEADER_CAPS_COMPLEX | DDS_HEADER_CAPS_MIPMAP;
			}

			unsigned int bitsPerPixelOrBlock = get_bits_per_pixel_or_block(format);

			if (is_compressed(format))
			{
				header.flags |= DDS_HEADER_FLAGS_LINEARSIZE;
				unsigned int blockWidth, blockHeight;
				get_block_size(format, blockWidth, blockHeight);
				header.pitchOrLinearSize = width * height * bitsPerPixelOrBlock / (8 * blockWidth * blockHeight);
			}
			else
			{
				header.flags |= DDS_HEADER_FLAGS_PITCH;
				header.pitchOrLinearSize = width * bitsPerPixelOrBlock / 8;
			}

			header.height = height;
			header.width = width;
			header.depth = depth;
			header.mipMapCount = mipCount;
			header.reserved1[0] = 1415075152;
			header.reserved1[1] = 4999506;
			header.reserved1[2] = header.reserved1[3] = header.reserved1[4] = 0;
			header.reserved1[5] = header.reserved1[6] = header.reserved1[7] = 0;
			header.reserved1[8] = header.reserved1[9] = header.reserved1[10] = 0;
		
			header.ddspf = get_pixelformat(format);

			// Unused
			header.caps3 = 0;
			header.caps4 = 0;
			header.reserved2 = 0;
		
			if (header.ddspf.fourCC == MakeFourCC("DX10"))
			{
				dxt10Header.dxgiFormat = format;
				dxt10Header.arraySize = arraySize;
				dxt10Header.miscFlag = 0;
		
				if (type == Texture1D)
				{
					dxt10Header.resourceDimension = DXGI_Texture1D;
				}
				else if (type == Texture2D)
				{
					dxt10Header.resourceDimension = DXGI_Texture2D;
				}
				else if (type == Cubemap)
				{
					dxt10Header.resourceDimension = DXGI_Texture2D;
					dxt10Header.miscFlag |= DXGI_MISC_FLAG_CUBEMAP;
					header.caps |= DDS_HEADER_CAPS_COMPLEX;
					header.caps2 |= DDS_HEADER_CAPS2_CUBEMAP | DDS_HEADER_CAPS2_CUBEMAP_ALLFACES;
				}
				else if (type == Texture3D)
				{
					dxt10Header.resourceDimension = DXGI_Texture3D;
					header.flags |= DDS_HEADER_FLAGS_VOLUME;
					header.caps2 |= DDS_HEADER_CAPS2_VOLUME;
				}
			
				// dxt10Header.miscFlag TODO Alpha Mode
				return true;
			}
			return false;
		}

		inline std::ostream& serialize(std::ostream& out, Header& ddsHeader)
		{
			out.write((char*)&ddsHeader.size, 4);
			out.write((char*)&ddsHeader.flags, 4);
			out.write((char*)&ddsHeader.height, 4);
			out.write((char*)&ddsHeader.width, 4);
			out.write((char*)&ddsHeader.pitchOrLinearSize, 4);
			out.write((char*)&ddsHeader.depth, 4);
			out.write((char*)&ddsHeader.mipMapCount, 4);
			out.write((char*)&ddsHeader.reserved1, 4 * 11);
			out.write((char*)&ddsHeader.ddspf.size, 4);
			out.write((char*)&ddsHeader.ddspf.flags, 4);
			out.write((char*)&ddsHeader.ddspf.fourCC, 4);
			out.write((char*)&ddsHeader.ddspf.RGBBitCount, 4);
			out.write((char*)&ddsHeader.ddspf.RBitMask, 4);
			out.write((char*)&ddsHeader.ddspf.GBitMask, 4);
			out.write((char*)&ddsHeader.ddspf.BBitMask, 4);
			out.write((char*)&ddsHeader.ddspf.ABitMask, 4);
			out.write((char*)&ddsHeader.caps, 4);
			out.write((char*)&ddsHeader.caps2, 4);
			out.write((char*)&ddsHeader.caps3, 4);
			out.write((char*)&ddsHeader.caps4, 4);
			out.write((char*)&ddsHeader.reserved2, 4);

			return out;
		}

		inline std::ostream& serialize(std::ostream& out, HeaderDXT10& ddsDX10Header)
		{
			out.write((char*)&ddsDX10Header.dxgiFormat, 4);
			out.write((char*)&ddsDX10Header.resourceDimension, 4);
			out.write((char*)&ddsDX10Header.miscFlag, 4);
			out.write((char*)&ddsDX10Header.arraySize, 4);
			out.write((char*)&ddsDX10Header.reserved, 4);

			return out;
		}
	
		// Returns the offset from the base pointer to the desired mip and slice
		// Slice is either a texture from an array, a face from a cubemap, or a 2D slice of a volume texture
		inline ddspp_constexpr unsigned int get_offset(const Descriptor& desc, const unsigned int mip, const unsigned int slice)
		{
			// The mip/slice arrangement is different between texture arrays and volume textures
			//
			// Arrays
			//  __________  _____  __  __________  _____  __  __________  _____  __ 
			// |          ||     ||__||          ||     ||__||          ||     ||__|
			// |          ||_____|    |          ||_____|    |          ||_____|
			// |          |           |          |           |          |
			// |__________|           |__________|           |__________|
			//
			// Volume
			//  __________  __________  __________  _____  _____  _____  __  __  __ 
			// |          ||          ||          ||     ||     ||     ||__||__||__|
			// |          ||          ||          ||_____||_____||_____|
			// |          ||          ||          |
			// |__________||__________||__________|
			//

			unsigned long long offset = 0;
			unsigned long long mip0Size = desc.depthPitch * 8; // Work in bits

			if (desc.type == Texture3D)
			{
				for (unsigned int m = 0; m < mip; ++m)
				{
					unsigned long long mipSize = mip0Size >> 2 * m;
					offset += mipSize * desc.numMips;
				}

				unsigned long long lastMip = mip0Size >> 2 * mip;

				offset += lastMip * slice;
			}
			else
			{
				unsigned long long mipChainSize = 0;

				for (unsigned int m = 0; m < desc.numMips; ++m)
				{
					unsigned long long mipSize = mip0Size >> 2 * m; // Divide by 2 in width and height
					mipChainSize += mipSize > desc.bitsPerPixelOrBlock ? mipSize : desc.bitsPerPixelOrBlock;
				}

				offset += mipChainSize * slice;

				for (unsigned int m = 0; m < mip; ++m)
				{
					unsigned long long mipSize = mip0Size >> 2 * m; // Divide by 2 in width and height
					offset += mipSize > desc.bitsPerPixelOrBlock ? mipSize : desc.bitsPerPixelOrBlock;
				}
			}

			offset /= 8; // Back to bytes

			return (unsigned int)offset;
		}
	}
}