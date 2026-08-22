#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor_factory.h"
#include "rlgl.h"
#include "tools.h"
#include "collision.h"
#include "assets.h"
#include "post_processing.h"

int draw_debug_info = FALSE;
int draw_collider_info = FALSE;

int light_count = 0;
Vector4 world_light_positions[MAX_LIGHTS] = { 0 };
Vector4 world_light_colors[MAX_LIGHTS] = { 0 };

Color clear_background_color;

RenderTexture2D render_tex_pre = { 0 };
RenderTexture2D render_tex_main = { 0 };
RenderTexture2D render_tex_post = { 0 };
RenderTexture2D render_tex_hud = { 0 };

const int renderlayers_enabled = FALSE;
Vector2 renderlayer_pos_background = { 0 };
Vector2 renderlayer_pos_tilemap = { 0 };
Vector2 renderlayer_pos_foreground = { 0 };
RenderTexture2D render_tex_background = { 0 };
RenderTexture2D render_tex_tilemap = { 0 };
RenderTexture2D render_tex_foreground = { 0 };

void game_draw(double tick_percent)
{
	// Recalculate render size
	float window_aspect = (float)screenWidth / (float)screenHeight;
	renderWidth = (int)((float)renderHeight * window_aspect);

	int want_resize = FALSE;
	if (IsRenderTextureValid(render_tex_pre))
	{
		// Check if we have a new texture size
		if (render_tex_pre.texture.width != renderWidth || render_tex_pre.texture.height != renderHeight)
			want_resize = TRUE;
	}
	else
	{
		// Invalid tex
		want_resize = TRUE;
	}
	
	if (want_resize)
	{
		UnloadRenderTextures();
		UnloadPostProcessingTextures();
		LoadRenderTextures();
		LoadPostProcessingTextures();
	}

	// Clear color affects fog
	Color draw_clear_col = Vector3ToColor(fog_color, 0.0f);

	////////////////////////////////////////////////////////////////////////
	// Predraw
	////////////////////////////////////////////////////////////////////////
	int has_main_draw = FALSE;
	int has_transparent_draw = FALSE;
	int has_main_postdraw = FALSE;
	int has_hud_predraw = FALSE;
	int has_hud_draw = FALSE;
	int has_hud_postdraw = FALSE;
	BeginTextureMode(render_tex_pre);
	ClearBackground(clear_background_color);
	BeginMode3D(cam_main);
	// Prep shaders
	rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
	BeginBlendMode(BLEND_CUSTOM_SEPARATE);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
			continue;
		if (ACTOR_HAS(draw_actor, func_predrawworld))
			draw_actor->func_predrawworld(draw_actor, tick_percent);
		// Check for future draw events. We don't need to loop over all actors in the future if we check now!
		if (ACTOR_HAS(draw_actor, func_drawworld))
			has_main_draw = TRUE;
		if(ACTOR_HAS(draw_actor, func_transparentdrawworld))
			has_transparent_draw = TRUE;
		if (ACTOR_HAS(draw_actor, func_postdrawworld))
			has_main_postdraw = TRUE;
		if (ACTOR_HAS(draw_actor, func_predrawhud))
			has_hud_predraw = TRUE;
		if (ACTOR_HAS(draw_actor, func_drawhud))
			has_hud_draw = TRUE;
		if (ACTOR_HAS(draw_actor, func_postdrawhud))
			has_hud_postdraw = TRUE;
	}
	EndMode3D();
	EndTextureMode();

	if (!draw_collider_info)
	{
		////////////////////////////////////////////////////////////////////////
		// Main draw
		////////////////////////////////////////////////////////////////////////
		BeginTextureMode(render_tex_main);
		ClearBackground(draw_clear_col);
		if (has_main_draw)
		{
			BeginMode3D(cam_main);
			rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
			BeginBlendMode(BLEND_CUSTOM_SEPARATE);
			// Base pass
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_drawworld))
					draw_actor->func_drawworld(draw_actor, tick_percent);
			}
			EndMode3D();
		}
		if (has_transparent_draw)
		{
			BeginMode3D(cam_main);
			rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
			BeginBlendMode(BLEND_CUSTOM_SEPARATE);
			// Transparent pass
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_transparentdrawworld))
					draw_actor->func_transparentdrawworld(draw_actor, tick_percent);
			}
			EndMode3D();
		}
		EndTextureMode();


		////////////////////////////////////////////////////////////////////////
		// Post draw
		////////////////////////////////////////////////////////////////////////

		BeginTextureMode(render_tex_post);
		ClearBackground(draw_clear_col);
		if (has_main_postdraw)
		{
			BeginMode3D(cam_main);
			rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
			BeginBlendMode(BLEND_CUSTOM_SEPARATE);
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_postdrawworld))
					draw_actor->func_postdrawworld(draw_actor, tick_percent);
			}
			EndMode3D();
		}
		EndTextureMode();
	}
	else
	{
		////////////////////////////////////////////////////////////////////////
		// Collider debug drawing
		////////////////////////////////////////////////////////////////////////

		BeginTextureMode(render_tex_main);
		ClearBackground(draw_clear_col);
		BeginMode3D(cam_main);
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);
		// Get collision material
		Material* mat_col = AssetGet_Material(ASSET_MATERIALS"/Engine/collision_debug.mat");
		for (int i = 0; i <= max_collision; i++)
		{
			struct ColliderData* col = GetCollider(i);
			Matrix position = GetMatrix(col->owner);
			DrawMesh(
				*col->mesh,
				*mat_col,
				position
			);
		}
		EndMode3D();
		EndTextureMode();

		// Now draw the actors as sprites
		BeginTextureMode(render_tex_post);
		ClearBackground(draw_clear_col);
		BeginMode3D(cam_main);
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);
		Model* sprite_model = AssetGet_Model(ASSET_MODELS"/Tools/sprite2d.glb");
		mat_col = AssetGet_Material(ASSET_MATERIALS"/Engine/no_texture.mat");
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (draw_actor->actor_type == act_camera)
				continue;
			Transform facing_transform = {
				.translation = draw_actor->position,
				.rotation = QuaternionLookAt(cam_main.position, draw_actor->position, VEC3UP),
				.scale = Vector3Scale(Vector3One(),0.2f)
			};
			DrawMesh(
				sprite_model->meshes[0],
				*mat_col,
				MATRIX_ASSEMBLE(facing_transform)
			);
		}
		// Draw raycast lines
		for (int i = 0; i < debug_current_rays; i++)
		{
			DrawLine3D(debug_ray_starts[i], debug_ray_ends[i], debug_ray_hits[i] ? RED : BLUE);
		}
		EndMode3D();
		EndTextureMode();
	}

	////////////////////////////////////////////////////////////////////////
	// Hud drawing
	////////////////////////////////////////////////////////////////////////

	BeginTextureMode(render_tex_hud);
	ClearBackground((Color) {20,20,20,0}); // Slightly gray
	BeginMode2D(cam_hud);
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);
		// predraw hud
		if (has_hud_predraw)
		{
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_predrawhud))
					draw_actor->func_predrawhud(draw_actor, tick_percent);
			}
		}
		// draw hud
		if (has_hud_draw)
		{
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_drawhud))
					draw_actor->func_drawhud(draw_actor, tick_percent);
			}
		}
		// postdraw hud
		if (has_hud_postdraw)
		{
			for (int i = 0; i <= current_actor_cap; i++)
			{
				struct Actor* draw_actor = world_actors[i];
				if (!ACTOR_EXISTS(draw_actor))
					continue;
				if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
					continue;
				if (ACTOR_HAS(draw_actor, func_postdrawhud))
					draw_actor->func_postdrawhud(draw_actor, tick_percent);
			}
		}
	}
	EndMode2D();
	EndTextureMode();

	// Assemble main drawing textures and render layers
	Vector2 org = (Vector2){ renderWidth / 2.0f, renderHeight / 2.0f };
	Rectangle src = (Rectangle){ 0, 0, (float)renderWidth, (float)-renderHeight };
	Rectangle dest = (Rectangle){ renderWidth / 2.0f, renderHeight / 2.0f, (float)renderWidth, (float)renderHeight };
	Rectangle layer_src = (Rectangle){ 0, 0, (float)RENDER_LAYER_SIZE, (float)-RENDER_LAYER_SIZE };
	BeginTextureMode(render_tex_postworld);
	ClearBackground((Color) { 0, 0, 0, 0 }); // Full Clear
	BeginMode2D(cam_hud);
	{
		BeginDrawing();
		BeginMode2D(cam_hud);
		// Background
		if (renderlayers_enabled)
			DrawTexturePro(render_tex_background.texture,
				layer_src,
				(Rectangle) {
			renderlayer_pos_background.x, renderlayer_pos_background.y, (float)RENDER_LAYER_SIZE, (float)-RENDER_LAYER_SIZE
		}, org, 0, WHITE);
		DrawTexturePro(render_tex_pre.texture, src, dest, org, 0, WHITE);
		// Mainground
		DrawTexturePro(render_tex_main.texture, src, dest, org, 0, WHITE);
		if (renderlayers_enabled)
			DrawTexturePro(render_tex_tilemap.texture,
				layer_src,
				(Rectangle) {
			renderlayer_pos_tilemap.x, renderlayer_pos_tilemap.y, (float)RENDER_LAYER_SIZE, (float)-RENDER_LAYER_SIZE
		}, org, 0, WHITE);
		// Foreground
		DrawTexturePro(render_tex_post.texture, src, dest, org, 0, WHITE);
		if (renderlayers_enabled)
			DrawTexturePro(render_tex_foreground.texture,
				layer_src,
				(Rectangle) {
			renderlayer_pos_foreground.x, renderlayer_pos_foreground.y, (float)RENDER_LAYER_SIZE, (float)-RENDER_LAYER_SIZE
		}, org, 0, WHITE);
	}
	EndMode2D();
	EndTextureMode();

	// Prepare the hud for post-processing
	BeginTextureMode(render_tex_posthud);
	ClearBackground((Color) { 0, 0, 0, 0 }); // Full Clear
	BeginMode2D(cam_hud);
	{
		DrawTexturePro(render_tex_hud.texture, src, dest, org, 0, WHITE);
	}
	EndMode2D();
	EndTextureMode();

	// Post processing handling, hud and world are seperate here!
	HandleWorldPostProcessing(&render_tex_postworld, src, dest, org);
	HandleHudPostProcessing(&render_tex_posthud, src, dest, org);

	// Final render pass
	BeginTextureMode(render_tex_postfinal);
	ClearBackground((Color) { 0, 0, 0, 0 }); // Full Clear
	BeginMode2D(cam_hud);
	{
		DrawTexturePro(render_tex_postworld.texture, src, dest, org, 0, WHITE);
		DrawTexturePro(render_tex_posthud.texture, src, dest, org, 0, WHITE);
	}
	EndMode2D();
	EndTextureMode();

	// Handle last post-processing step, applies to all renders at once
	HandleFinalPostProcessing(&render_tex_postfinal, src, dest, org);

	// Final draw
	org = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	src = (Rectangle){ 0, 0, (float)renderWidth, (float)-renderHeight };
	dest = (Rectangle){ screenWidth / 2.0f, screenHeight / 2.0f, (float)screenWidth, (float)screenHeight };
	BeginMode2D(cam_hud);
	{
		DrawTexturePro(render_tex_postfinal.texture, src, dest, org, 0, WHITE);
	}
	EndMode2D();

	EndDrawing();
}

void fog_set(Color col, float power, float dist)
{
	fog_distance = dist;
	fog_power = power;
	fog_color = (Vector3){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f };
}

void lighting_append_light(Vector3 pos, float radius, Color col, float influence)
{
	/*
	// Calculate range affected intensity
	if (col.r + col.g + col.b > 0 && radius < LIGHT_WORLD_RANGE) // Unless it's cave darkness or the skybox...
	{
		float dist = Vector3Distance(cam_main.position, pos);
		influence *= (1.0f - powf(dist / DEFAULT_MAX_RENDER_RANGE, 7.0f));
		if (influence <= 0) // Do not bother with lights we will never see.
			return;
	}
	*/
	int write_index = light_count;
	if (light_count >= MAX_LIGHTS) // Find furthest light and replace it
	{
		int weakest_index = 0;
		float weakest_influence = INFINITY;
		Vector3 cam_pos = cam_main.position;
		for (int i = 0; i < MAX_LIGHTS; i += 1)
		{
			// Radius multiplied by alpha
			float influence = world_light_positions[i].w * world_light_colors[i].w;
			// Check distance to camera by radius of light, global lights don't do this check.
			if (world_light_positions[i].w < LIGHT_WORLD_RANGE)
			{
				float dist = Clamp(
								Vector3Distance(cam_main.position, (Vector3) { world_light_positions[i].x, world_light_positions[i].y, world_light_positions[i].z }) / world_light_positions[i].w, 
								0, 
								1
							);
				influence *= dist;
			}
			if (influence >= weakest_influence)
				continue;
			// Weaker than the weakest so far
			weakest_influence = influence;
			weakest_index = i;
		}
		write_index = weakest_index;
	}
	// Write the light to the slot decided
	world_light_positions[write_index] = (Vector4){ pos.x, pos.y, pos.z, radius };
	world_light_colors[write_index] = (Vector4){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, influence };
	if (++light_count > MAX_LIGHTS)
		light_count = MAX_LIGHTS;
}

void shader_update_defaultuniforms(Shader shader, struct Actor* actor)
{
	int pos_loc = GetShaderLocation(shader, "uWorldPos");
	SetShaderValue(shader, pos_loc, &actor->position, SHADER_UNIFORM_VEC3);
}

void shader_update_fog(Shader shader)
{
	int fog_loc = GetShaderLocation(shader, "uFogColor");
	SetShaderValue(shader, fog_loc, &fog_color, SHADER_UNIFORM_VEC3);
	fog_loc = GetShaderLocation(shader, "uFogPower");
	SetShaderValue(shader, fog_loc, &fog_power, SHADER_UNIFORM_FLOAT);
	fog_loc = GetShaderLocation(shader, "uFogDistance");
	SetShaderValue(shader, fog_loc, &fog_distance, SHADER_UNIFORM_FLOAT);
}

void shader_update_lights(Shader shader)
{
	// Pass number of lights
	int light_loc = GetShaderLocation(shader, "uLightCount");
	if (light_loc < 0)
		return;
	SetShaderValue(shader, light_loc, &light_count, SHADER_UNIFORM_INT);

	light_loc = GetShaderLocation(shader, "uLightPositions");
	SetShaderValueV(shader, light_loc, &world_light_positions, SHADER_UNIFORM_VEC4, light_count);
	light_loc = GetShaderLocation(shader, "uLightColors");
	SetShaderValueV(shader, light_loc, &world_light_colors, SHADER_UNIFORM_VEC4, light_count);
}

void ToaDrawMesh(Asset* model_asset, int mesh_index, Material material, Matrix matrix, int show_backface)
{
	// Backface culling
	if(show_backface)
		rlDisableBackfaceCulling();
	// Draw
	DrawMesh(
		model_asset->mdl->meshes[mesh_index],
		material,
		matrix
	);
	// Restore backface mode
	if (show_backface)
		rlEnableBackfaceCulling();
}

void DrawAllModelMeshes(struct Actor* actor, char* model_path, char* material_paths[])
{
	Asset* model_asset = AssetGetPackage(model_path);
	if (model_asset == NULL)
		return;

	int index = 0;
	size_t iter = 0;
	void* item;
	while (hashmap_iter(model_asset->mesh_data, &iter, &item)) {
		const MeshInfo* search_mesh = item;
		char* mat_path = material_paths[index];
		STANDARD_SHADER_MATERIAL(current_mat, mat_path, actor);
		ToaDrawMesh(
			model_asset,
			GetMeshIndex(model_asset->mesh_data, search_mesh->mesh_name),
			*current_mat,
			GetMatrix(actor),
			FALSE
		);
		index++;
	}
}

void LoadRenderTextures()
{
	render_tex_pre = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_main = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_post = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_hud = LoadRenderTexture(renderWidth, renderHeight);
}

void UnloadRenderTextures()
{
	if (IsRenderTextureValid(render_tex_pre))
		UnloadRenderTexture(render_tex_pre);
	if (IsRenderTextureValid(render_tex_main))
		UnloadRenderTexture(render_tex_main);
	if (IsRenderTextureValid(render_tex_post))
		UnloadRenderTexture(render_tex_post);
	if (IsRenderTextureValid(render_tex_hud))
		UnloadRenderTexture(render_tex_hud);
}

void LoadRenderLayers()
{
	if (!renderlayers_enabled)
		return;
	render_tex_background = LoadRenderTexture(RENDER_LAYER_SIZE, RENDER_LAYER_SIZE);
	render_tex_tilemap = LoadRenderTexture(RENDER_LAYER_SIZE, RENDER_LAYER_SIZE);
	render_tex_foreground = LoadRenderTexture(RENDER_LAYER_SIZE, RENDER_LAYER_SIZE);
	SetTextureWrap(render_tex_background.texture, TEXTURE_WRAP_REPEAT);
	SetTextureWrap(render_tex_tilemap.texture, TEXTURE_WRAP_REPEAT);
	SetTextureWrap(render_tex_foreground.texture, TEXTURE_WRAP_REPEAT);
}

void UnloadRenderLayers()
{
	if (IsRenderTextureValid(render_tex_background))
		UnloadRenderTexture(render_tex_background);
	if (IsRenderTextureValid(render_tex_tilemap))
		UnloadRenderTexture(render_tex_tilemap);
	if (IsRenderTextureValid(render_tex_foreground))
		UnloadRenderTexture(render_tex_foreground);
}