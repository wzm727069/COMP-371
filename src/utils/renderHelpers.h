
#include <iostream>
#include <string>
#define MAX_TEX 32
#include "../Common.h"

static void resetTextures() 
{
	for (unsigned int i = 0; i < MAX_TEX; i++)
	{
		if(g_textures[i].renderer_id <32 && g_textures[i].renderer_id > 0)
			g_textures[i].setBound(false);
	}
}


static void bindTextures()
{
	for (unsigned int i = 0; i < MAX_TEX; i++)
	{
		if (g_textures[i].renderer_id < 32 && g_textures[i].renderer_id > 0)
		{
			g_textures[i].bind(g_texLocations[i]);


		}
	}
}