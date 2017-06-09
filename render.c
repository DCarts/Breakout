/*
 * global.c
 * 
 * Copyright 2017 Daniel <dcsouza@dcc.ufrj.br>
 *                Guilherme <guiavenas@ufrj.br>
 *                Gabriel <gabrielizotongo@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"
#include "global.h"
#include "render.h"

static SDL_Rect srcRect, dstRect;

static SDL_Color scoreFontColor = {255,255,255};
static SDL_Color bgColor = {0,0,0};

int render() {
	int i, err = false;

	//Fill the surface white
	SDL_FillRect( gScreenSurface, NULL,
	SDL_MapRGB( gScreenSurface->format,
				0, 0, 0 ) );

	srcRect.x = 24; srcRect.y = 24;
	srcRect.w = 592; srcRect.h = 432;
	SDL_FillRect( gScreenSurface, &srcRect,
	SDL_MapRGB( gScreenSurface->format,
				0xBB, 0xBB, 0xBB ) );

	srcRect.x = 32; srcRect.y = 32;
	srcRect.w = 576; srcRect.h = 416;
	SDL_FillRect( gScreenSurface, &srcRect,
	SDL_MapRGB( gScreenSurface->format,
				0, 0, 0 ) );


	/* Renderiza os blocos */
	for(i = 0; i < gNumBlocos; i++) {
		if (gBlocos[i].vida){
			srcRect.w = gBlocos[i].w;
			srcRect.h = gBlocos[i].h;

			dstRect.x = gBlocos[i].pos.x;
			dstRect.y = gBlocos[i].pos.y;
			
			srcRect.x = 0; srcRect.y = 0;

			if( SDL_BlitSurface( gBlocos[i].img, &srcRect,
								gScreenSurface, &dstRect ) < 0 ) {
				fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
				err = true;
			}
			
			if (gBlocos[i].vida < gBlocos[i].maxVida){
				srcRect.x = 32 * (4 - gBlocos[i].vida); srcRect.y = 0;
				
				if( SDL_BlitSurface( gBlocoBreak, &srcRect,
									gScreenSurface, &dstRect ) < 0 ) {
					fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
					err = true;
				}
			}

		}

	}

	/* Renderiza as bolas */
	srcRect.x = 0; srcRect.y = 0;
	
	for(i = 0; i < gNumBolas; i++) {
		srcRect.w = gBolas[i].dim;
		srcRect.h = gBolas[i].dim;

		dstRect.x = gBolas[i].pos.x;
		dstRect.y = gBolas[i].pos.y;

		if (gBolas[i].ativo){
			if( SDL_BlitSurface( gBolas[i].img, &srcRect,
								gScreenSurface, &dstRect ) < 0 ) {
				fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
				err = true;
			}
		}
	}

	/* Renderiza a plataforma */
	srcRect.w = gPad[0].w;
	srcRect.h = gPad[0].h;

	dstRect.x = gPad[0].pos.x;
	dstRect.y = gPad[0].pos.y;

	if( SDL_BlitSurface( gPad[0].img, &srcRect,
							gScreenSurface, &dstRect ) < 0 ) {
			fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
            err = true;
		}

	err = renderScoreboard();

    //Update the surface
    SDL_UpdateWindowSurface( gWindow );

	return err;
}

int renderScoreboard() {
	int err = false;
	static int lastScore = -1;
	static char scoreText[8];
	
	if (gPlayer.pontos != lastScore) {
		lastScore = gPlayer.pontos;
		sprintf(scoreText, "%d", lastScore);
		SDL_FreeSurface(gScoreSurface);
		free(gScoreSurface);
		gScoreSurface = NULL;
		if(!(gScoreSurface = TTF_RenderText_Shaded(gScoreFonte, scoreText, scoreFontColor, bgColor))) {
			fprintf(stderr,"Impossivel renderizar texto na tela! %s\n",TTF_GetError());
			err = true;
		}
	}

	dstRect.x = gScoreOffset;
	dstRect.y = 16;
	dstRect.w = 128;
	dstRect.h = 48;

	if (SDL_BlitSurface(gScoreSurface, NULL, gScreenSurface, &dstRect) < 0) {
		fprintf(stderr,"Impossivel blitar texto na tela! %s\n",SDL_GetError());
		err = true;
	}
	return err;
}
