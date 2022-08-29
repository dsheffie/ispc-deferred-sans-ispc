/*
  Copyright (c) 2011, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef DEFERRED_H
#define DEFERRED_H

// Currently tile widths must be a multiple of SIMD width (i.e. 8 for ispc sse4x2)!
#define MIN_TILE_WIDTH 16
#define MIN_TILE_HEIGHT 16
#define MAX_LIGHTS 1024

enum InputDataArraysEnum {
    idaZBuffer = 0,
    idaNormalEncoded_x,
    idaNormalEncoded_y,
    idaSpecularAmount,
    idaSpecularPower,
    idaAlbedo_x,
    idaAlbedo_y,
    idaAlbedo_z,
    idaLightPositionView_x,
    idaLightPositionView_y,
    idaLightPositionView_z,
    idaLightAttenuationBegin,
    idaLightColor_x,
    idaLightColor_y,
    idaLightColor_z,
    idaLightAttenuationEnd,

    idaNum
};

#ifndef ISPC

#include <stdint.h>

#define ALIGNMENT_BYTES 64

#define MAX_LIGHTS 1024

#define VISUALIZE_LIGHT_COUNT 0

struct InputDataArrays
{
  float *zBuffer;
  uint16_t *normalEncoded_x; // half float
  uint16_t *normalEncoded_y; // half float
  uint16_t *specularAmount; // half float
  uint16_t *specularPower; // half float
  uint8_t *albedo_x; // unorm8
  uint8_t *albedo_y; // unorm8
  uint8_t *albedo_z; // unorm8
  float *lightPositionView_x;
  float *lightPositionView_y;
  float *lightPositionView_z;
  float *lightAttenuationBegin;
  float *lightColor_x;
  float *lightColor_y;
  float *lightColor_z;
  float *lightAttenuationEnd;
};

struct InputHeader
{
  float cameraProj[4][4];
  float cameraNear;
  float cameraFar;
  
  int32_t framebufferWidth;
  int32_t framebufferHeight;
  int32_t numLights;
  int32_t inputDataChunkSize;
  int32_t inputDataArrayOffsets[idaNum];
};

struct InputData {
  InputHeader header;
  InputDataArrays arrays;
  uint8_t *chunk;
};

struct Framebuffer {
    Framebuffer(int width, int height);
    ~Framebuffer();

    void clear();

    uint8_t *r, *g, *b;

  private:
    int nPixels;
    Framebuffer(const Framebuffer &);
    Framebuffer &operator=(const Framebuffer *);
};

InputData *CreateInputDataFromFile(const char *path);
void DeleteInputData(InputData *input);
void WriteFrame(const char *filename, const InputData *input, const Framebuffer &framebuffer);
void InitDynamicC(InputData *input);
void DispatchDynamicC(InputData *input, Framebuffer *framebuffer);

#endif // !ISPC

#endif // DEFERRED_H
