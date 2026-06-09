#include "animation.h"
#include "raymath.h"
#include "raylib.h"
#include "rlgl.h"       // Requried for: rlUpdateVertexBuffer() (CPU-skinning)
#include <stdlib.h>     // Required for: NULL



/*******************************************************************************************
*
*   raylib [models] example - animation blend custom [EXERPT]
*
*   Example complexity rating: [★★★★] 4/4
*
*   Example originally created with raylib 5.5, last time updated with raylib 6.0
*
*   Example contributed by dmitrii-brand (@dmitrii-brand) and reviewed by Ramon Santamaria (@raysan5)
*
*   DETAILS: Example demonstrates per-bone animation blending, allowing smooth transitions
*   between two animations by interpolating bone transforms. This is useful for:
*    - Blending movement animations (walk/run) with action animations (jump/attack)
*    - Creating smooth animation transitions
*    - Layering animations (e.g., upper body attack while lower body walks)
*
*   WARNING: GPU skinning must be enabled in raylib with a compilation flag,
*   if not enabled, CPU skinning will be used instead
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2026 dmitrii-brand (@dmitrii-brand)
*
********************************************************************************************/

void SkinModel(Model* model)
{
    for (int m = 0; m < model->meshCount; m++)
    {
        Mesh mesh = model->meshes[m];
        Vector3 animVertex = { 0 };
        Vector3 animNormal = { 0 };
        const int vertexValuesCount = mesh.vertexCount * 3;

        int boneIndex = 0;
        int boneCounter = 0;
        float boneWeight = 0.0f;
        bool bufferUpdateRequired = false; // Flag to check when anim vertex information is updated

        // Skip if missing bone data or missing anim buffers initialization
        if ((mesh.boneWeights == NULL) || (mesh.boneIndices == NULL) ||
            (mesh.animVertices == NULL) || (mesh.animNormals == NULL)) continue;

        for (int vCounter = 0; vCounter < vertexValuesCount; vCounter += 3)
        {
            mesh.animVertices[vCounter] = 0;
            mesh.animVertices[vCounter + 1] = 0;
            mesh.animVertices[vCounter + 2] = 0;
            if (mesh.animNormals != NULL)
            {
                mesh.animNormals[vCounter] = 0;
                mesh.animNormals[vCounter + 1] = 0;
                mesh.animNormals[vCounter + 2] = 0;
            }

            // Iterates over 4 bones per vertex
            for (int j = 0; j < 4; j++, boneCounter++)
            {
                boneWeight = mesh.boneWeights[boneCounter];
                boneIndex = mesh.boneIndices[boneCounter];

                // Early stop when no transformation will be applied
                if (boneWeight == 0.0f) continue;
                animVertex = (Vector3){ mesh.vertices[vCounter], mesh.vertices[vCounter + 1], mesh.vertices[vCounter + 2] };
                animVertex = Vector3Transform(animVertex, model->boneMatrices[boneIndex]);
                mesh.animVertices[vCounter] += animVertex.x * boneWeight;
                mesh.animVertices[vCounter + 1] += animVertex.y * boneWeight;
                mesh.animVertices[vCounter + 2] += animVertex.z * boneWeight;
                bufferUpdateRequired = true;

                // Normals processing
                // NOTE: We use meshes.baseNormals (default normal) to calculate meshes.normals (animated normals)
                if ((mesh.normals != NULL) && (mesh.animNormals != NULL))
                {
                    animNormal = (Vector3){ mesh.normals[vCounter], mesh.normals[vCounter + 1], mesh.normals[vCounter + 2] };
                    animNormal = Vector3Transform(animNormal, MatrixTranspose(MatrixInvert(model->boneMatrices[boneIndex])));
                    mesh.animNormals[vCounter] += animNormal.x * boneWeight;
                    mesh.animNormals[vCounter + 1] += animNormal.y * boneWeight;
                    mesh.animNormals[vCounter + 2] += animNormal.z * boneWeight;
                }
            }
        }

        if (bufferUpdateRequired)
        {
            // Update GPU vertex buffers with updated data (position + normals)
            rlUpdateVertexBuffer(mesh.vboId[SHADER_LOC_VERTEX_POSITION], mesh.animVertices, mesh.vertexCount * 3 * sizeof(float), 0);
            if (mesh.normals != NULL) rlUpdateVertexBuffer(mesh.vboId[SHADER_LOC_VERTEX_NORMAL], mesh.animNormals, mesh.vertexCount * 3 * sizeof(float), 0);
        }
    }
}