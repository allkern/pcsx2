/*
 *	Copyright (C) 2011-2013 Gregory hainaut
 *	Copyright (C) 2007-2009 Gabest
 *
 *  This file was generated by glsl2h.pl script
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include "stdafx.h"

extern const char* tfx_glsl =
	"//#version 420 // Keep it for text editor detection\n"
	"\n"
	"// note lerp => mix\n"
	"\n"
	"#define FMT_32 0\n"
	"#define FMT_24 1\n"
	"#define FMT_16 2\n"
	"#define FMT_PAL 4 /* flag bit */\n"
	"\n"
	"// Not sure we have same issue on opengl. Doesn't work anyway on ATI card\n"
	"// And I say this as an ATI user.\n"
	"#define ATI_SUCKS 0\n"
	"\n"
	"#ifndef VS_BPPZ\n"
	"#define VS_BPPZ 0\n"
	"#define VS_TME 1\n"
	"#define VS_FST 1\n"
	"#define VS_LOGZ 0\n"
	"#endif\n"
	"\n"
	"#ifndef GS_IIP\n"
	"#define GS_IIP 0\n"
	"#define GS_PRIM 3\n"
	"#endif\n"
	"\n"
	"#ifndef PS_FST\n"
	"#define PS_FST 0\n"
	"#define PS_WMS 0\n"
	"#define PS_WMT 0\n"
	"#define PS_FMT FMT_32\n"
	"#define PS_AEM 0\n"
	"#define PS_TFX 0\n"
	"#define PS_TCC 1\n"
	"#define PS_ATST 1\n"
	"#define PS_FOG 0\n"
	"#define PS_CLR1 0\n"
	"#define PS_FBA 0\n"
	"#define PS_AOUT 0\n"
	"#define PS_LTF 1\n"
	"#define PS_COLCLIP 0\n"
	"#define PS_DATE 0\n"
	"#define PS_SPRITEHACK 0\n"
	"#define PS_POINT_SAMPLER 0\n"
	"#define PS_TCOFFSETHACK 0\n"
	"#endif\n"
	"\n"
	"struct vertex\n"
	"{\n"
	"    vec4 p;\n"
	"    vec4 t;\n"
	"    vec4 tp;\n"
	"    vec4 c;\n"
	"};\n"
	"\n"
	"#ifdef VERTEX_SHADER\n"
	"layout(location = 0) in vec2  i_st;\n"
	"layout(location = 1) in vec4  i_c;\n"
	"layout(location = 2) in float i_q;\n"
	"layout(location = 3) in uvec2 i_p;\n"
	"layout(location = 4) in uint  i_z;\n"
	"layout(location = 5) in uvec2 i_uv;\n"
	"layout(location = 6) in vec4  i_f;\n"
	"\n"
	"layout(location = 0) out vertex VSout;\n"
	"\n"
	"out gl_PerVertex {\n"
	"    invariant vec4 gl_Position;\n"
	"    float gl_PointSize;\n"
	"    float gl_ClipDistance[];\n"
	"};\n"
	"\n"
	"#ifdef DISABLE_GL42\n"
	"layout(std140) uniform cb20\n"
	"#else\n"
	"layout(std140, binding = 20) uniform cb20\n"
	"#endif\n"
	"{\n"
	"    vec4 VertexScale;\n"
	"    vec4 VertexOffset;\n"
	"    vec2 TextureScale;\n"
	"};\n"
	"\n"
	"void vs_main()\n"
	"{\n"
	"    uint z;\n"
	"    if(VS_BPPZ == 1) // 24\n"
	"        z = i_z & uint(0xffffff);\n"
	"    else if(VS_BPPZ == 2) // 16\n"
	"        z = i_z & uint(0xffff);\n"
	"    else\n"
	"        z = i_z;\n"
	"\n"
	"    // pos -= 0.05 (1/320 pixel) helps avoiding rounding problems (integral part of pos is usually 5 digits, 0.05 is about as low as we can go)\n"
	"    // example: ceil(afterseveralvertextransformations(y = 133)) => 134 => line 133 stays empty\n"
	"    // input granularity is 1/16 pixel, anything smaller than that won't step drawing up/left by one pixel\n"
	"    // example: 133.0625 (133 + 1/16) should start from line 134, ceil(133.0625 - 0.05) still above 133\n"
	"\n"
	"    vec4 p = vec4(i_p, z, 0) - vec4(0.05f, 0.05f, 0, 0); \n"
	"    vec4 final_p = p * VertexScale - VertexOffset;\n"
	"    // FIXME\n"
	"    // FLIP vertically\n"
	"    final_p.y *= -1.0f;\n"
	"\n"
	"    if(VS_LOGZ == 1)\n"
	"    {\n"
	"        final_p.z = log2(1.0f + float(z)) / 32.0f;\n"
	"    }\n"
	"\n"
	"    VSout.p = final_p;\n"
	"    gl_Position = final_p; // NOTE I don't know if it is possible to merge POSITION_OUT and gl_Position\n"
	"#if VS_RTCOPY\n"
	"    VSout.tp = final_p * vec4(0.5, -0.5, 0, 0) + 0.5;\n"
	"#endif\n"
	"\n"
	"\n"
	"    if(VS_TME != 0)\n"
	"    {\n"
	"        if(VS_FST != 0)\n"
	"        {\n"
	"            //VSout.t.xy = i_t * TextureScale;\n"
	"            VSout.t.xy = i_uv * TextureScale;\n"
	"            VSout.t.w = 1.0f;\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            //VSout.t.xy = i_t;\n"
	"            VSout.t.xy = i_st;\n"
	"            VSout.t.w = i_q;\n"
	"        }\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        VSout.t.xy = vec2(0.0f, 0.0f);\n"
	"        VSout.t.w = 1.0f;\n"
	"    }\n"
	"\n"
	"    VSout.c = i_c;\n"
	"    VSout.t.z = i_f.r;\n"
	"}\n"
	"\n"
	"#endif\n"
	"\n"
	"#ifdef GEOMETRY_SHADER\n"
	"in gl_PerVertex {\n"
	"    vec4 gl_Position;\n"
	"    float gl_PointSize;\n"
	"    float gl_ClipDistance[];\n"
	"} gl_in[];\n"
	"\n"
	"out gl_PerVertex {\n"
	"    vec4 gl_Position;\n"
	"    float gl_PointSize;\n"
	"    float gl_ClipDistance[];\n"
	"};\n"
	"\n"
	"layout(location = 0) in vertex GSin[];\n"
	"\n"
	"layout(location = 0) out vertex GSout;\n"
	"\n"
	"#if GS_PRIM == 0\n"
	"layout(points) in;\n"
	"layout(points, max_vertices = 1) out;\n"
	"\n"
	"void gs_main()\n"
	"{\n"
	"    for(int i = 0; i < gl_in.length(); i++) {\n"
	"        gl_Position = gl_in[i].gl_Position; // FIXME is it useful\n"
	"        GSout = GSin[i];\n"
	"        EmitVertex();\n"
	"    }\n"
	"    EndPrimitive();\n"
	"}\n"
	"\n"
	"#elif GS_PRIM == 1\n"
	"layout(lines) in;\n"
	"layout(line_strip, max_vertices = 2) out;\n"
	"\n"
	"void gs_main()\n"
	"{\n"
	"    for(int i = 0; i < gl_in.length(); i++) {\n"
	"        gl_Position = gl_in[i].gl_Position; // FIXME is it useful\n"
	"        GSout = GSin[i];\n"
	"#if GS_IIP == 0\n"
	"        if (i == 0)\n"
	"            GSout.c = GSin[1].c;\n"
	"#endif\n"
	"        EmitVertex();\n"
	"    }\n"
	"    EndPrimitive();\n"
	"}\n"
	"\n"
	"#elif GS_PRIM == 2\n"
	"layout(triangles) in;\n"
	"layout(triangle_strip, max_vertices = 3) out;\n"
	"\n"
	"void gs_main()\n"
	"{\n"
	"    for(int i = 0; i < gl_in.length(); i++) {\n"
	"        gl_Position = gl_in[i].gl_Position; // FIXME is it useful\n"
	"        GSout = GSin[i];\n"
	"#if GS_IIP == 0\n"
	"        if (i == 0 || i == 1)\n"
	"            GSout.c = GSin[2].c;\n"
	"#endif\n"
	"        EmitVertex();\n"
	"    }\n"
	"    EndPrimitive();\n"
	"}\n"
	"\n"
	"#elif GS_PRIM == 3\n"
	"layout(lines) in;\n"
	"layout(triangle_strip, max_vertices = 6) out;\n"
	"\n"
	"void gs_main()\n"
	"{\n"
	"    // left top     => GSin[0];\n"
	"    // right bottom => GSin[1];\n"
	"    vertex rb = GSin[1];\n"
	"    vertex lt = GSin[0];\n"
	"\n"
	"    lt.p.z = rb.p.z;\n"
	"    lt.t.zw = rb.t.zw;\n"
	"#if GS_IIP == 0\n"
	"    lt.c = rb.c;\n"
	"#endif\n"
	"\n"
	"    vertex lb = rb;\n"
	"    lb.p.x = lt.p.x;\n"
	"    lb.t.x = lt.t.x;\n"
	"\n"
	"    vertex rt = rb;\n"
	"    rt.p.y = lt.p.y;\n"
	"    rt.t.y = lt.t.y;\n"
	"\n"
	"    // Triangle 1\n"
	"    gl_Position = lt.p;\n"
	"    GSout = lt;\n"
	"    EmitVertex();\n"
	"\n"
	"    gl_Position = lb.p;\n"
	"    GSout = lb;\n"
	"    EmitVertex();\n"
	"\n"
	"    gl_Position = rt.p;\n"
	"    GSout = rt;\n"
	"    EmitVertex();\n"
	"\n"
	"    EndPrimitive();\n"
	"\n"
	"    // Triangle 2\n"
	"    gl_Position = lb.p;\n"
	"    GSout = lb;\n"
	"    EmitVertex();\n"
	"\n"
	"    gl_Position = rt.p;\n"
	"    GSout = rt;\n"
	"    EmitVertex();\n"
	"\n"
	"    gl_Position = rb.p;\n"
	"    GSout = rb;\n"
	"    EmitVertex();\n"
	"\n"
	"    EndPrimitive();\n"
	"\n"
	"}\n"
	"\n"
	"#endif\n"
	"\n"
	"#endif\n"
	"\n"
	"#ifdef FRAGMENT_SHADER\n"
	"layout(location = 0) in vertex PSin;\n"
	"\n"
	"// Same buffer but 2 colors for dual source blending\n"
	"layout(location = 0, index = 0) out vec4 SV_Target0;\n"
	"layout(location = 0, index = 1) out vec4 SV_Target1;\n"
	"\n"
	"layout(binding = 0) uniform sampler2D TextureSampler;\n"
	"layout(binding = 1) uniform sampler2D PaletteSampler;\n"
	"layout(binding = 2) uniform sampler2D RTCopySampler;\n"
	"\n"
	"#ifdef DISABLE_GL42\n"
	"layout(std140) uniform cb21\n"
	"#else\n"
	"layout(std140, binding = 21) uniform cb21\n"
	"#endif\n"
	"{\n"
	"    vec3 FogColor;\n"
	"    float AREF;\n"
	"    vec4 HalfTexel;\n"
	"    vec4 WH;\n"
	"    vec4 MinMax;\n"
	"    vec2 MinF;\n"
	"    vec2 TA;\n"
	"    uvec4 MskFix;\n"
	"    vec4 TC_OffsetHack;\n"
	"};\n"
	"\n"
	"vec4 sample_c(vec2 uv)\n"
	"{\n"
	"    // FIXME: check the issue on openGL\n"
	"	if (ATI_SUCKS == 1 && PS_POINT_SAMPLER == 1)\n"
	"	{\n"
	"		// Weird issue with ATI cards (happens on at least HD 4xxx and 5xxx),\n"
	"		// it looks like they add 127/128 of a texel to sampling coordinates\n"
	"		// occasionally causing point sampling to erroneously round up.\n"
	"		// I'm manually adjusting coordinates to the centre of texels here,\n"
	"		// though the centre is just paranoia, the top left corner works fine.\n"
	"		uv = (trunc(uv * WH.zw) + vec2(0.5, 0.5)) / WH.zw;\n"
	"	}\n"
	"\n"
	"    // FIXME I'm not sure it is a good solution to flip texture\n"
	"    return texture(TextureSampler, uv);\n"
	"    //FIXME another way to FLIP vertically\n"
	"    //return texture(TextureSampler, vec2(uv.x, 1.0f-uv.y) );\n"
	"}\n"
	"\n"
	"vec4 sample_p(float u)\n"
	"{\n"
	"    //FIXME do we need a 1D sampler. Big impact on opengl to find 1 dim\n"
	"    // So for the moment cheat with 0.0f dunno if it work\n"
	"    return texture(PaletteSampler, vec2(u, 0.0f));\n"
	"}\n"
	"\n"
	"vec4 sample_rt(vec2 uv)\n"
	"{\n"
	"    return texture(RTCopySampler, uv);\n"
	"}\n"
	"\n"
	"vec4 wrapuv(vec4 uv)\n"
	"{\n"
	"    vec4 uv_out = uv;\n"
	"\n"
	"    if(PS_WMS == PS_WMT)\n"
	"    {\n"
	"        if(PS_WMS == 2)\n"
	"        {\n"
	"            uv_out = clamp(uv, MinMax.xyxy, MinMax.zwzw);\n"
	"        }\n"
	"        else if(PS_WMS == 3)\n"
	"        {\n"
	"            uv_out = vec4(((ivec4(uv * WH.xyxy) & ivec4(MskFix.xyxy)) | ivec4(MskFix.zwzw)) / WH.xyxy);\n"
	"        }\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        if(PS_WMS == 2)\n"
	"        {\n"
	"            uv_out.xz = clamp(uv.xz, MinMax.xx, MinMax.zz);\n"
	"        }\n"
	"        else if(PS_WMS == 3)\n"
	"        {\n"
	"            uv_out.xz = vec2(((ivec2(uv.xz * WH.xx) & ivec2(MskFix.xx)) | ivec2(MskFix.zz)) / WH.xx);\n"
	"        }\n"
	"        if(PS_WMT == 2)\n"
	"        {\n"
	"            uv_out.yw = clamp(uv.yw, MinMax.yy, MinMax.ww);\n"
	"        }\n"
	"        else if(PS_WMT == 3)\n"
	"        {\n"
	"            uv_out.yw = vec2(((ivec2(uv.yw * WH.yy) & ivec2(MskFix.yy)) | ivec2(MskFix.ww)) / WH.yy);\n"
	"        }\n"
	"    }\n"
	"\n"
	"    return uv_out;\n"
	"}\n"
	"\n"
	"vec2 clampuv(vec2 uv)\n"
	"{\n"
	"    vec2 uv_out = uv;\n"
	"\n"
	"    if(PS_WMS == 2 && PS_WMT == 2) \n"
	"    {\n"
	"        uv_out = clamp(uv, MinF, MinMax.zw);\n"
	"    }\n"
	"    else if(PS_WMS == 2)\n"
	"    {\n"
	"        uv_out.x = clamp(uv.x, MinF.x, MinMax.z);\n"
	"    }\n"
	"    else if(PS_WMT == 2)\n"
	"    {\n"
	"        uv_out.y = clamp(uv.y, MinF.y, MinMax.w);\n"
	"    }\n"
	"\n"
	"    return uv_out;\n"
	"}\n"
	"\n"
	"mat4 sample_4c(vec4 uv)\n"
	"{\n"
	"    mat4 c;\n"
	"\n"
	"    c[0] = sample_c(uv.xy);\n"
	"    c[1] = sample_c(uv.zy);\n"
	"    c[2] = sample_c(uv.xw);\n"
	"    c[3] = sample_c(uv.zw);\n"
	"\n"
	"    return c;\n"
	"}\n"
	"\n"
	"vec4 sample_4a(vec4 uv)\n"
	"{\n"
	"    vec4 c;\n"
	"\n"
	"    // XXX\n"
	"    // I think .a is related to 8bit texture in alpha channel\n"
	"    // Opengl is only 8 bits on red channel. Not sure exactly of the impact\n"
	"    c.x = sample_c(uv.xy).a;\n"
	"    c.y = sample_c(uv.zy).a;\n"
	"    c.z = sample_c(uv.xw).a;\n"
	"    c.w = sample_c(uv.zw).a;\n"
	"\n"
	"	return c * 255./256 + 0.5/256;\n"
	"}\n"
	"\n"
	"mat4 sample_4p(vec4 u)\n"
	"{\n"
	"    mat4 c;\n"
	"\n"
	"    c[0] = sample_p(u.x);\n"
	"    c[1] = sample_p(u.y);\n"
	"    c[2] = sample_p(u.z);\n"
	"    c[3] = sample_p(u.w);\n"
	"\n"
	"    return c;\n"
	"}\n"
	"\n"
	"vec4 sample_color(vec2 st, float q)\n"
	"{\n"
	"    if(PS_FST == 0) st /= q;\n"
	"\n"
	"    if(PS_TCOFFSETHACK == 1) st += TC_OffsetHack.xy;\n"
	"\n"
	"    vec4 t;\n"
	"    mat4 c;\n"
	"    vec2 dd;\n"
	"\n"
	"    if (PS_LTF == 0 && PS_FMT <= FMT_16 && PS_WMS < 3 && PS_WMT < 3)\n"
	"    {\n"
	"        c[0] = sample_c(clampuv(st));\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        vec4 uv;\n"
	"\n"
	"        if(PS_LTF != 0)\n"
	"        {\n"
	"            uv = st.xyxy + HalfTexel;\n"
	"            dd = fract(uv.xy * WH.zw);\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            uv = st.xyxy;\n"
	"        }\n"
	"\n"
	"        uv = wrapuv(uv);\n"
	"\n"
	"        if((PS_FMT & FMT_PAL) != 0)\n"
	"        {\n"
	"            c = sample_4p(sample_4a(uv));\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            c = sample_4c(uv);\n"
	"        }\n"
	"    }\n"
	"\n"
	"    // PERF: see the impact of the exansion before/after the interpolation\n"
	"    for (int i = 0; i < 4; i++)\n"
	"    {\n"
	"        if((PS_FMT & ~FMT_PAL) == FMT_24)\n"
	"        {\n"
	"            // FIXME GLSL any only support bvec so try to mix it with notEqual\n"
	"            bvec3 rgb_check = notEqual( t.rgb, vec3(0.0f, 0.0f, 0.0f) );\n"
	"            t.a = ( (PS_AEM == 0) || any(rgb_check)  ) ? TA.x : 0.0f;\n"
	"        }\n"
	"        else if((PS_FMT & ~FMT_PAL) == FMT_16)\n"
	"        {\n"
	"            // FIXME GLSL any only support bvec so try to mix it with notEqual\n"
	"            bvec3 rgb_check = notEqual( t.rgb, vec3(0.0f, 0.0f, 0.0f) );\n"
	"            t.a = t.a >= 0.5 ? TA.y : ( (PS_AEM == 0) || any(rgb_check) ) ? TA.x : 0.0f;\n"
	"        }\n"
	"    }\n"
	"\n"
	"    if(PS_LTF != 0)\n"
	"    {\n"
	"        t = mix(mix(c[0], c[1], dd.x), mix(c[2], c[3], dd.x), dd.y);\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        t = c[0];\n"
	"    }\n"
	"\n"
	"    return t;\n"
	"}\n"
	"\n"
	"vec4 tfx(vec4 t, vec4 c)\n"
	"{\n"
	"    vec4 c_out = c;\n"
	"    if(PS_TFX == 0)\n"
	"    {\n"
	"        if(PS_TCC != 0) \n"
	"        {\n"
	"            c_out = c * t * 255.0f / 128;\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            c_out.rgb = c.rgb * t.rgb * 255.0f / 128;\n"
	"        }\n"
	"    }\n"
	"    else if(PS_TFX == 1)\n"
	"    {\n"
	"        if(PS_TCC != 0) \n"
	"        {\n"
	"            c_out = t;\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            c_out.rgb = t.rgb;\n"
	"        }\n"
	"    }\n"
	"    else if(PS_TFX == 2)\n"
	"    {\n"
	"        c_out.rgb = c.rgb * t.rgb * 255.0f / 128 + c.a;\n"
	"\n"
	"        if(PS_TCC != 0) \n"
	"        {\n"
	"            c_out.a += t.a;\n"
	"        }\n"
	"    }\n"
	"    else if(PS_TFX == 3)\n"
	"    {\n"
	"        c_out.rgb = c.rgb * t.rgb * 255.0f / 128 + c.a;\n"
	"\n"
	"        if(PS_TCC != 0) \n"
	"        {\n"
	"            c_out.a = t.a;\n"
	"        }\n"
	"    }\n"
	"\n"
	"    return clamp(c_out, vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));\n"
	"}\n"
	"\n"
	"void datst()\n"
	"{\n"
	"#if PS_DATE > 0\n"
	"    float alpha = sample_rt(PSin.tp.xy).a;\n"
	"    float alpha0x80 = 128. / 255;\n"
	"\n"
	"    if (PS_DATE == 1 && alpha >= alpha0x80)\n"
	"        discard;\n"
	"    else if (PS_DATE == 2 && alpha < alpha0x80)\n"
	"        discard;\n"
	"#endif\n"
	"}\n"
	"\n"
	"void atst(vec4 c)\n"
	"{\n"
	"    float a = trunc(c.a * 255 + 0.01);\n"
	"\n"
	"    if(PS_ATST == 0) // never\n"
	"    {\n"
	"        discard;\n"
	"    }\n"
	"    else if(PS_ATST == 1) // always\n"
	"    {\n"
	"        // nothing to do\n"
	"    }\n"
	"    else if(PS_ATST == 2 ) // l\n"
	"    {\n"
	"        if (PS_SPRITEHACK == 0)\n"
	"            if ((AREF - a - 0.5f) < 0.0f)\n"
	"                discard;\n"
	"    }\n"
	"    else if(PS_ATST == 3 ) // le\n"
	"    {\n"
	"        if ((AREF - a + 0.5f) < 0.0f)\n"
	"            discard;\n"
	"    }\n"
	"    else if(PS_ATST == 4) // e\n"
	"    {\n"
	"        if ((0.5f - abs(a - AREF)) < 0.0f)\n"
	"            discard;\n"
	"    }\n"
	"    else if(PS_ATST == 5) // ge\n"
	"    {\n"
	"        if ((a-AREF + 0.5f) < 0.0f)\n"
	"            discard;\n"
	"    }\n"
	"    else if(PS_ATST == 6) // g\n"
	"    {\n"
	"        if ((a-AREF - 0.5f) < 0.0f)\n"
	"            discard;\n"
	"    }\n"
	"    else if(PS_ATST == 7) // ne\n"
	"    {\n"
	"        if ((abs(a - AREF) - 0.5f) < 0.0f)\n"
	"            discard;\n"
	"    }\n"
	"}\n"
	"\n"
	"vec4 fog(vec4 c, float f)\n"
	"{\n"
	"    vec4 c_out = c;\n"
	"    if(PS_FOG != 0)\n"
	"    {\n"
	"        c_out.rgb = mix(FogColor, c.rgb, f);\n"
	"    }\n"
	"\n"
	"    return c_out;\n"
	"}\n"
	"\n"
	"vec4 ps_color()\n"
	"{\n"
	"    datst();\n"
	"\n"
	"    vec4 t = sample_color(PSin.t.xy, PSin.t.w);\n"
	"\n"
	"    vec4 c = tfx(t, PSin.c);\n"
	"\n"
	"    atst(c);\n"
	"\n"
	"    c = fog(c, PSin.t.z);\n"
	"\n"
	"    if (PS_COLCLIP == 2)\n"
	"    {\n"
	"        c.rgb = 256.0f/255.0f - c.rgb;\n"
	"    }\n"
	"    if (PS_COLCLIP > 0)\n"
	"    {\n"
	"        // FIXME !!!!\n"
	"        //c.rgb *= c.rgb < 128./255;\n"
	"        bvec3 factor = bvec3(128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);\n"
	"        c.rgb *= vec3(factor);\n"
	"    }\n"
	"\n"
	"    if(PS_CLR1 != 0) // needed for Cd * (As/Ad/F + 1) blending modes\n"
	"    {\n"
	"        c.rgb = vec3(1.0f, 1.0f, 1.0f); \n"
	"    }\n"
	"\n"
	"    return c;\n"
	"}\n"
	"\n"
	"void ps_main()\n"
	"{\n"
	"    //FIXME\n"
	"    vec4 c = ps_color();\n"
	"\n"
	"    // FIXME: I'm not sure about the value of others field\n"
	"    // output.c1 = c.a * 2; // used for alpha blending\n"
	"\n"
	"    float alpha = c.a * 2;\n"
	"\n"
	"    if(PS_AOUT != 0) // 16 bit output\n"
	"    {\n"
	"        float a = 128.0f / 255; // alpha output will be 0x80\n"
	"\n"
	"        c.a = (PS_FBA != 0) ? a : step(0.5, c.a) * a;\n"
	"    }\n"
	"    else if(PS_FBA != 0)\n"
	"    {\n"
	"        if(c.a < 0.5) c.a += 0.5;\n"
	"    }\n"
	"\n"
	"    SV_Target0 = c;\n"
	"    SV_Target1 = vec4(alpha, alpha, alpha, alpha);\n"
	"}\n"
	"#endif\n"
	;
