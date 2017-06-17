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

int render() {
	SDL_Rect srcRect, dstRect;
	SDL_Rect ttfRect; /*	Retangulo para o texto TTF */
	int i, err = false;
	char pontuacao[11];

	/* Dimensoes do texto TTF */
	ttfRect.x = gScreenWidth - gScoreBoardWidth;
	ttfRect.y = OFFSET;
	ttfRect.w = gScoreBoardWidth;
	ttfRect.h = gScoreBoardHeight;


	/* Desenha area de jogo */
	SDL_FillRect( gScreenSurface, NULL,
	SDL_MapRGB( gScreenSurface->format,
				0, 0, 0 ) );

	srcRect.x = 24; srcRect.y = 24;
	srcRect.w = 592; srcRect.h = 456;
	SDL_FillRect( gScreenSurface, &srcRect,
	SDL_MapRGB( gScreenSurface->format,
				0xBB, 0xBB, 0xBB ) );

	srcRect.x = 32; srcRect.y = 32;
	srcRect.w = 576; srcRect.h = 440;
	SDL_FillRect( gScreenSurface, &srcRect,
	SDL_MapRGB( gScreenSurface->format,
				0, 0, 0 ) );

	/* Renderiza os blocos */
	for(i = 0; i < gNumBlocos; i++) {
		if (gBlocos[i].vida){
			
			srcRect.y = 0;
			srcRect.w = gBlocos[i].w;
			srcRect.h = gBlocos[i].h;
			
			srcRect.x = 0; srcRect.y = 0;
			
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

	/* Renderiza os leds */
	srcRect.x = 0; srcRect.y = 0;

	for(i = 0; i < gMaxVidas; i++) {
		srcRect.w = 24;
		srcRect.h = 8;

		dstRect.x = 624 + i*32;
		dstRect.y = gScreenHeight - 2*OFFSET;

		if (i+1 <= gPlayer.vidas){
			if( SDL_BlitSurface( gLed[0], &srcRect,
								gScreenSurface, &dstRect) < 0 ) {
				fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
				err = true;
			}
		}else{
			if( SDL_BlitSurface( gLed[1], &srcRect,
								gScreenSurface, &dstRect) < 0 ) {
				fprintf(stderr, "Erro: SDL nao blitou: %s\n", SDL_GetError() );
				err = true;
			}
		}
	}
	
	
	/* Renderiza as bolas */
	for(i = 0; i < gNumBolas; i++) {
		srcRect.w = gBolas[i].dim;
		srcRect.h = gBolas[i].dim;

		dstRect.x = gBolas[i].pos.x;
		dstRect.y = gBolas[i].pos.y;

		if (gBolas[i].ativo){
			if( SDL_BlitSurface( gBolas[i].img, &srcRect,
								gScreenSurface, &dstRect) < 0 ) {
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

		/*	Renderiza o texto TTF	*/
		if(!(gTexto = TTF_RenderText_Shaded(gFonte, gPlayer.nome, corDaFonte, backgroundColor))){
			fprintf(stderr,"Impossivel renderizar texto na tela! %s\n",TTF_GetError());
			err = true;
		}

		if (SDL_BlitSurface(gTexto, NULL, gScreenSurface, &ttfRect) < 0) {
			fprintf(stderr,"Impossivel blitar texto na tela! %s\n",SDL_GetError());
			err = true;

		}

		ttfRect.x = gScreenWidth - gScoreBoardWidth;
		ttfRect.y = OFFSET + gScoreBoardHeight + 15;
		ttfRect.w = gScoreBoardWidth;
		ttfRect.h = gScoreBoardHeight;

		sprintf(pontuacao,"%d",gPlayer.pontos);

		if(!(gPontos = TTF_RenderText_Shaded(gFonte,pontuacao,corDaFonte,backgroundColor))){
			fprintf(stderr,"Impossivel renderizar pontuação na tela! %s\n",TTF_GetError());
			err = true;
		}


		if (SDL_BlitSurface(gPontos, NULL, gScreenSurface, &ttfRect) < 0) {
			fprintf(stderr,"Impossivel blitar pontuação na tela! %s\n",SDL_GetError());
			err = true;
		}


    /*	Update the surface	*/
    SDL_UpdateWindowSurface( gWindow );

	SDL_FreeSurface(gPontos);
	SDL_FreeSurface(gTexto);

	return err;
}