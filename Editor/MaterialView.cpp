#include "stdafx.h"
#include "Editor.h"
#include "MaterialView.h"

#include "MaterialDoc.h"
#include "MaterialPreviewPage.h"
#include "StringUtils.h"

#include "OgreString.h"

const char gProgram0[] =
    "abstract alpha_op_ex alpha_rejection alpha_to_coverage ambient anim_texture attach binding_type cg colour_op colour_op_ex colour_op_multipass_fallback colour_write column_major_matrices compositor compositor_logic content_type cubic_texture cull_hardware cull_software default_params delegate depth_bias depth_check depth_func depth_write diffuse emissive entry_point env_map filtering fog_override fragment_program fragment_program_ref from geometry_program geometry_program_ref glsl gpu_device_rule gpu_vendor_rule hlsl illumination_stage import includes_morph_animation includes_pose_animation includes_skeletal_animation input input_operation_type iteration iteration_depth_bias light_clip_planes light_scissor lighting lod_distances lod_index manual_named_constants material max_anisotropy max_lights max_output_vertices mipmap_bias normalise_normals output_operation_type param_indexed param_indexed_auto param_named param_named_auto pass point_size point_size_attenuation point_size_max point_size_min point_sprites polygon_mode polygon_mode_overrideable preprocessor_defines profiles receive_shadows render_quad rotate rotate_anim scale scene_blend scheme scheme scroll scroll_anim separate_scene_blend set set_texture_alias shading shadow_caster_material shadow_caster_vertex_program_ref shadow_receiver_fragment_program_ref shadow_receiver_material shadow_receiver_vertex_program_ref source specular start_light syntax target target target_output technique tex_address_mode tex_border_colour tex_coord_set texture texture texture_alias texture_unit transform transparency_casts_shadows transparent_sorting uses_adjacency_information uses_vertex_texture_fetch vertex_program vertex_program_ref wave_xform";
const char gProgram1[] =
    "1d 1d 2d 2d 3d 3d 4d add add_signed add_smooth alpha_blend always_fail always_pass ambient_light_colour amplitude animation_parametric anisotropic anticlockwise arbfp1 arbvp1 back base bilinear blend_current_alpha blend_diffuse_alpha blend_diffuse_colour blend_manual blend_texture_alpha border camera_position camera_position_object_space chain_scope clamp clockwise colour colour_blend combinedUVW costime_0_1 costime_0_2pi costime_0_x cubic cubic_normal cubic_reflection custom density depth_pool derived_ambient_light_colour derived_light_diffuse_colour derived_light_diffuse_colour_array derived_light_specular_colour derived_light_specular_colour_array derived_scene_colour dest_alpha dest_colour directional dotproduct double end equal exclude exp exp2 false far_clip_distance flat float float2 float3 float3x3 float3x4 float4 float4x3 float4x4 fog_colour fog_params fov fp20 fp30 fp40 fps fragment frame_time frequency front gamma global_scope glslf glslv gouraud gp4fp gp4gp gp4vp greater greater_equal half include int inverse_projection_matrix inverse_sawtooth inverse_texture_size inverse_transpose_projection_matrix inverse_transpose_view_matrix inverse_transpose_viewproj_matrix inverse_transpose_world_matrix inverse_transpose_worldview_matrix inverse_transpose_worldviewproj_matrix inverse_view_matrix inverse_viewport_height inverse_viewport_width inverse_viewproj_matrix inverse_world_matrix inverse_worldview_matrix inverse_worldviewproj_matrix less less_equal light_attenuation light_attenuation_array light_casts_shadows light_count light_diffuse_colour light_diffuse_colour_array light_diffuse_colour_power_scaled light_diffuse_colour_power_scaled_array light_direction light_direction_array light_direction_object_space light_direction_object_space_array light_direction_view_space light_direction_view_space_array light_distance_object_space light_distance_object_space_array light_number light_position light_position_array light_position_object_space light_position_object_space_array light_position_view_space light_position_view_space_array light_power light_power_array light_specular_colour light_specular_colour_array light_specular_colour_power_scaled light_specular_colour_power_scaled_array linear local_scope lod_camera_position lod_camera_position_object_space mirror modulate modulate_x2 modulate_x4 named near_clip_distance no_fsaa none not_equal off on once once_per_light one one_minus_dest_alpha one_minus_dest_colour one_minus_src_alpha one_minus_src_colour packed_texture_size pass_iteration_number pass_number per_light per_n_lights PF_A1R5G5B5 PF_A2B10G10R10 PF_A2R10G10B10 PF_A4L4 PF_A4R4G4B4 PF_A8 PF_A8B8G8R8 PF_A8R8G8B8 PF_A8R8G8B8 PF_B5G6R5 PF_B8G8R8 PF_B8G8R8A8 PF_BYTE_LA PF_FLOAT16_R PF_FLOAT16_R PF_FLOAT16_RGB PF_FLOAT16_RGB PF_FLOAT16_RGBA PF_FLOAT16_RGBA PF_FLOAT32_R PF_FLOAT32_R PF_FLOAT32_RGB PF_FLOAT32_RGB PF_FLOAT32_RGBA PF_FLOAT32_RGBA PF_L16 PF_L8 PF_R3G3B2 PF_R5G6B5 PF_R8G8B8 PF_R8G8B8 PF_R8G8B8A8 PF_R8G8B8A8 PF_SHORT_RGBA PF_X8B8G8R8 PF_X8R8G8B8 phase phong planar point points pooled previous projection_matrix ps_1_1 ps_1_2 ps_1_3 ps_1_4 ps_2_0 ps_2_a ps_2_b ps_2_x ps_3_0 ps_3_x render_target_flipping replace sawtooth scale_x scale_y scaled scene_depth_range scope scroll_x scroll_y separateUV shadow shadow_colour shadow_extrusion_distance shadow_scene_depth_range sine sintime_0_1 sintime_0_2pi sintime_0_x solid source1 source2 spherical spot spotlight_params spotlight_params_array spotlight_viewproj_matrix spotlight_worldviewproj_matrix square src_alpha src_colour src_current src_diffuse src_manual src_specular src_texture start subtract surface_ambient_colour surface_diffuse_colour surface_emissive_colour surface_shininess surface_specular_colour tantime_0_1 tantime_0_2pi tantime_0_x target_height target_height target_width target_width_scaled texel_offsets texture_matrix texture_size texture_viewproj_matrix texture_viewproj_matrix_array texture_worldviewproj_matrix texture_worldviewproj_matrix_array time_0_1 time_0_1_packed time_0_2pi time_0_2pi_packed time_0_x time_0_x_packed transpose_projection_matrix transpose_view_matrix transpose_viewproj_matrix transpose_world_matrix transpose_worldview_matrix transpose_worldviewproj_matrix triangle trilinear true type vertex vertexcolour view_direction view_matrix view_side_vector view_up_vector viewport_height viewport_size viewport_width viewproj_matrix vp20 vp30 vp40 vs_1_1 vs_2_0 vs_2_a vs_2_x vs_3_0 wave_type wireframe world_matrix world_matrix_array_3x4 worldview_matrix worldviewproj_matrix wrap xform_type zero";

const char gCG0[] = 
	"asm asm_frgment auto BCOL0 BCOL1 BINORMAL BLENDINDICES BLENDWEIGHT break case catch char class COLOR COLOR0 COLOR1 COLOR2 COLOR3 column_major compile const const_cast continue decl default delete DEPTH discard do double double1x1 double1x2 double1x3 double1x4 double2 double2 double2x1 double2x3 double2x4 double3 double3x1 double3x2 double3x3 double3x4 double4 double4x1 double4x2 double4x3 double4x4 dword dynamic_cast else emit enum explicit extern FACE FALSE fixed float float float1x1 float1x2 float1x3 float1x4 float2 float2x1 float2x2 float2x3 float2x4 float3 float3x1 float3x2 float3x3 float3x4 float4 float4x1 float4x2 float4x3 float4x4 FOG FOGCOORD for friend get goto half half half1x1 half1x2 half1x3 half1x4 half2 half2x1 half2x2 half2x3 half2x4 half3 half3x1 half3x2 half3x3 half3x4 half4 half4x1 half4x2 half4x3 half4x4 if in inline inout int int1x1 int1x2 int1x3 int1x4 int2 int2x1 int2x2 int2x3 int2x4 int3 int3x1 int3x2 int3x3 int3x4 int4 int4x1 int4x2 int4x3 int4x4 interface long matrix1x1 matrix1x2 matrix1x3 matrix1x4 matrix2x1 matrix2x2 matrix2x3 matrix2x4 matrix3x1 matrix3x2 matrix3x3 matrix3x4 matrix4x1 matrix4x2 matrix4x3 matrix4x4 mutable namespace new NORMAL operator out packed pass pixelfragment pixelshader POSITION private protected PSIZE public register reinterpret_cast return row_major sampler sampler1D sampler2D sampler3D sampler_state samplerCUBE shared short short short1x1 short1x1 short1x2 short1x2 short1x3 short1x3 short1x4 short1x4 short2 short2 short2x1 short2x1 short2x2 short2x2 short2x3 short2x3 short2x4 short2x4 short3 short3 short3x1 short3x1 short3x2 short3x2 short3x3 short3x3 short3x4 short3x4 short4 short4 short4x1 short4x1 short4x2 short4x2 short4x3 short4x3 short4x4 short4x4 signed sizeof static static_cast string struct switch TANGENT technique template TESSFACTOR TEXCOORD0 TEXCOORD1 TEXCOORD10 TEXCOORD11 TEXCOORD12 TEXCOORD13 TEXCOORD14 TEXCOORD15 TEXCOORD2 TEXCOORD3 TEXCOORD4 TEXCOORD5 TEXCOORD6 TEXCOORD7 TEXCOORD8 TEXCOORD9 texture texture1D texture2D texture3D textureCUBE textureRECT TEXUNIT0 TEXUNIT1 TEXUNIT10 TEXUNIT11 TEXUNIT12 TEXUNIT13 TEXUNIT14 TEXUNIT15 TEXUNIT2 TEXUNIT3 TEXUNIT4 TEXUNIT5 TEXUNIT6 TEXUNIT7 TEXUNIT8 TEXUNIT9 this throw TRUE try typedef typeid typename uniform union unsigned using vector vertexfragment vertexshader virtual void volatile while WPOS";
const char gCG1[] = 
	"abs acos all any asin atan atan2 ceil clamp cos cosh cross ddx ddy debug degrees determinant distance dot exp exp2 faceforward floor fmod frac frexp isfinite isinf isnan ldexp length lerp lit log log10 log2 max min modf mul mul mul noise normalize pow radians reflect refract round rsqrt saturate sin sincos sinh smoothstep sqrt step tan tanh tex1D tex1Dproj tex2D tex2Dproj tex3D tex3Dproj texCUBE texCUBEproj texRECT texRECTproj transpose";

const char gGLSL0[] = 
	"";
const char gGLSL1[] = 
	"";

const char gHLSL0[] = 
	"";
const char gHLSL1[] = 
	"";

IMPLEMENT_DYNCREATE(MaterialView, Scintilla::CScintillaView)

MaterialView::MaterialView()
{
	// mEdited = FALSE;
	// mTimer = FALSE;
	timer = false;
}

MaterialView::~MaterialView()
{
}

BEGIN_MESSAGE_MAP(MaterialView, Scintilla::CScintillaView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void MaterialView::OnDestroy()
{
	CScintillaView::OnDestroy();

	CString Extension = StringUtils::extension(std::string(GetDocument()->GetPathName())).c_str();

	if(Extension == "material")
	{
		CMaterialPreviewPage::Current->ClearMaterialList();
	} else

	if(Extension == "compositor")
	{
		CMaterialPreviewPage::Current->ClearCompositorList();
	}

	CMaterialPreviewPage::Current->SetCurrentFile(NULL);
}

void MaterialView::OnSetFocus(CWnd* pOldWnd)
{
	CScintillaView::OnSetFocus(pOldWnd);
	((MaterialDoc*)GetDocument())->setActiveView(this);
	parseFile();
}

void MaterialView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		timer = false;
		KillTimer(1);

		parseFile();
	}
}

void MaterialView::OnInitialUpdate()
{
	CScintillaView::OnInitialUpdate();

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	rCtrl.SetLexer(SCLEX_CPP);

	CString Extension = StringUtils::extension(std::string(GetDocument()->GetPathName())).c_str();

	if(Extension == "material" || Extension == "compositor")
	{
		rCtrl.SetKeyWords(0, gProgram0);
		rCtrl.SetKeyWords(1, gProgram1);
	} else

	if(Extension == "cg")
	{
		rCtrl.SetKeyWords(0, gCG0);
		rCtrl.SetKeyWords(1, gCG1);
	} else

	if(Extension == "glsl")
	{
		rCtrl.SetKeyWords(0, gGLSL0);
		rCtrl.SetKeyWords(1, gGLSL1);
	} else

	if(Extension == "hlsl")
	{
		rCtrl.SetKeyWords(0, gHLSL0);
		rCtrl.SetKeyWords(1, gHLSL1);
	}

	setAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 13, "Consolas"); // 字体
	rCtrl.SetMarginTypeN(0, SC_MARGIN_NUMBER); // 行号
	rCtrl.SetMarginWidthN(0, 48);

	rCtrl.StyleClearAll();
	setAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	setAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	setAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	setAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	setAStyle(SCE_C_WORD2, RGB(0x2B, 0x91, 0xAF));
	setAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
	setAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
	setAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	setAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

	rCtrl.SetCaretLineVisible(TRUE); // 当前行高亮
	rCtrl.SetCaretLineBack(RGB(0xff, 0xff, 0xb0));

	rCtrl.SetTabWidth(4);

	parseFile();
}

void MaterialView::OnCharAdded(Scintilla::SCNotification* pSCNotification)
{
	lastChar = pSCNotification->ch;

	CDocument *Doc = GetDocument();
	Doc->SetModifiedFlag();

	//if(!mEdited)
	//{
	//	mEdited = TRUE;
	//	CDocument *Doc = GetDocument();
	//	Doc->SetTitle(Doc->GetTitle() + "*");
	//}
}

void MaterialView::OnUpdateUI(Scintilla::SCNotification* pSCNotification)
{
	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	/** 在一段时间（30ms）内无动作则解析文件。
	*/
	if(!timer)
	{
		timer = true;
		SetTimer(1, 30, NULL);
	}
	else
	{
		timer = false;
		KillTimer(1);
	}

	//////////////////////////////////////////////////
	// 自动缩进
	//////////////////////////////////////////////////

	int pos = rCtrl.GetCurrentPos();
	int line = rCtrl.LineFromPosition(pos);
	if(lastChar !=0 && strchr("})>]", lastChar))
	{
		int startpos = rCtrl.WordStartPosition(pos-1, false);
		int linepos = rCtrl.PositionFromLine(line);
		if(startpos == linepos)
		{
			int othpos = rCtrl.BraceMatch(pos-1);
			int othline = rCtrl.LineFromPosition(othpos);
			int nIndent = rCtrl.GetLineIndentation(othline);
			char space[1024];
			memset(space,' ',1024);
			rCtrl.SetTargetStart(startpos);
			rCtrl.SetTargetEnd(pos-1);
			rCtrl.ReplaceTarget(nIndent, space);
		}
	}

	if(lastChar == '\n')
	{
		if(line > 0)
		{
			int nIndent = rCtrl.GetLineIndentation(line-1);
			int nPrevLinePos = rCtrl.PositionFromLine(line-1);
			
			int c = ' ';
			for(int p = pos-2; p>=nPrevLinePos; p--, c=rCtrl.GetCharAt(p));
			{
				if(c && strchr("{([<",c))
				{
					nIndent += 4;
				}
			}

			char space[1024];
			memset(space, ' ', 1024);
			space[nIndent] = 0;

			rCtrl.ReplaceSel(space);
		}
	}

	lastChar = 0;
}

void MaterialView::parseFile()
{
	CString PathName = GetDocument()->GetPathName();
	CString Extension;

	BOOL Flag = FALSE;
	for(int i=0; i<PathName.GetLength(); i++)
	{
		if(Flag)
			Extension += PathName[i];
		if(PathName[i] == '.')
			Flag = TRUE;
	}

	if(Extension == "material")
	{
		parseMaterialFile();
	}
	else if(Extension == "compositor")
	{
		parseCompositorFile();
	}
}

void MaterialView::parseMaterialFile()
{
	CDocument *Doc = GetDocument();
	CMaterialPreviewPage::Current->SetCurrentFile(Doc->GetPathName());

	CListBox *MaterialList = CMaterialPreviewPage::Current->GetMaterialList();
	MaterialList->ResetContent();

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();
	for(int i=0; i<rCtrl.GetLineCount(); i++)
	{
		Ogre::String Line = rCtrl.GetLine(i);

		Ogre::StringUtil::trim(Line);
		if(Ogre::StringUtil::startsWith(Line, "material "))
		{
			Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(Line, "\t\r\n: ");
			bool skipFirst = true, canPrint = true;
			std::string match, _match;
			for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
			{
				if (skipFirst)
				{
					skipFirst = false;
					continue;
				}

				std::string match = (*it);

				if(match.find_first_of('\"') != std::string::npos)
				{
					if(canPrint)
					{
						canPrint = false;
					}
					else
					{
						canPrint = true;
						match = _match + match;
					}
				}
				
				if(canPrint)
				{
					Ogre::StringUtil::trim(match);
					match.erase(0, match.find_first_not_of('\"'));  
					match.erase(match.find_last_not_of('\"')+1);  

					if(!match.empty())
					{
						MaterialList->InsertString(MaterialList->GetCount(), match.c_str());
					}
				}
				else
				{
					_match += (match + " ");
				}
			}
		}
	}
}

void MaterialView::parseCompositorFile()
{
	CDocument *Doc = GetDocument();
	CMaterialPreviewPage::Current->SetCurrentFile(Doc->GetPathName());

	CListBox *CompositorList = CMaterialPreviewPage::Current->GetCompositorList();
	CompositorList->ResetContent();

	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();
	for(int i=0; i<rCtrl.GetLineCount(); i++)
	{
		Ogre::String Line = rCtrl.GetLine(i);

		Ogre::StringUtil::trim(Line);
		if(Ogre::StringUtil::startsWith(Line, "compositor "))
		{
			Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(Line, "\t\r\n: ");
			bool skipFirst = true, canPrint = true;
			std::string match, _match;
			for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
			{
				if (skipFirst)
				{
					skipFirst = false;
					continue;
				}

				std::string match = (*it);

				if(match.find_first_of('\"') != std::string::npos)
				{
					if(canPrint)
					{
						canPrint = false;
					}
					else
					{
						canPrint = true;
						match = _match + match;
					}
				}
				
				if(canPrint)
				{
					Ogre::StringUtil::trim(match);
					match.erase(0, match.find_first_not_of('\"'));  
					match.erase(match.find_last_not_of('\"')+1);  

					if(!match.empty())
					{
						CompositorList->InsertString(CompositorList->GetCount(), match.c_str());
					}
				}
				else
				{
					_match += (match + " ");
				}
			}
		}
	}
}

void MaterialView::setAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
	Scintilla::CScintillaCtrl &rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1)
		rCtrl.StyleSetSize(style, size);
	if (face) 
		rCtrl.StyleSetFont(style, face);
}
