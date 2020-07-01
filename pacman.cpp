#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cstdlib>

using namespace std;

const float FPS = 5;
const int SCREEN_W = 560;
const int SCREEN_H = 700;
const int BOUNCER_SIZE = 20;

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//matriz definindo mapa do jogo: 2 representa as bolinhas, 1 representa paredes, 0 representa corredor

char MAPA[32][29] = {
         "1111111111111111111111111111",
         "1222222222112222112222222221",
         "1211112112112112112112111121",
         "1211112112112112112112111121",
         "1222222112222112222112222221",
         "1211112111112112111112111121",
         "1211112111112112111112111121",
         "1211222222222112222222221121",
         "1211211110111111110111121121",
         "1211211110111111110111121121",
         "3222222000000000000002222223",
         "1211112110111331110112111121",
         "1211112110133333310112111121",
         "1211112110133333310112111121",
         "1222222110133333310112222221",
         "1011112110111111110112111101",
         "1011112110000000000112111101",
         "1000112110111111110112110001",
         "1110112110111111110112110111",
         "1110112222222112222222110111",
         "3000112111112112111112110003",
         "1011112111112112111112111101",
         "1011112112222202222112111101",
         "1222222112112112112112222221",
         "1211211112112112112111121121",
         "1211211112112112112111121121",
         "1211222222112222112222221121",
         "1211111112111111112111111121",
         "1211111112111111112111111121",
         "1222222222222222222222222221",
         "1111111111111111111111111111",
      };

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_BITMAP *pacman   = NULL;
ALLEGRO_BITMAP *pacmandir   = NULL;
ALLEGRO_BITMAP *bolinha   = NULL;
ALLEGRO_BITMAP *final   = NULL;
ALLEGRO_AUDIO_STREAM *musica  = NULL;
ALLEGRO_SAMPLE *sombolinha =  NULL;
ALLEGRO_SAMPLE *somderrota =  NULL;
ALLEGRO_SAMPLE *somvitoria =  NULL;
ALLEGRO_BITMAP *fantasminha1 =  NULL;
ALLEGRO_BITMAP *fantasminha2 =  NULL;
ALLEGRO_BITMAP *fantasminha3 =  NULL;
ALLEGRO_BITMAP *fantasminha4 =  NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_BITMAP *derrota   = NULL;

int i = 22, j = 14; //posicao inicial do Pacman na matriz
int q = 20; //tamanho de cada celula no mapa
int posy = i*q;
int posx = j*q;
int k1 = 13, k2 = 13, k3 = 10, k4 = 16;
int l1 = 14, l2 = 14, l3 = 10, l4 = 15;
int posfy1=k1*q;
int posfy2=k2*q;  
int posfy3=k3*q;
int posfy4=k4*q;
int posfx1=l1*q;
int posfx2=l2*q;
int posfx3=l3*q;
int posfx4=l4*q;
int movf1; 
int movf2;
int movf3;
int movf4;
int d=0,e=0,c=0,b=0;


int pontuacao = 0;

bool key[4] = { false, false, false, false };
bool redraw = true;
bool sair = false;
bool keyf[4] = { false, false, false, false };

int inicializa() {
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("statics/imagens/mapacman.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    pacman = al_load_bitmap("statics/imagens/yoda.bmp");
    if(!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);
    pacmandir = al_load_bitmap("statics/imagens/yodadir.bmp");
    if(!pacmandir)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);
    
    bolinha = al_load_bitmap("statics/imagens/bolinha.bmp");
    
    if(!bolinha)
    {
        cout << "Falha ao carregar a bolinha!" << endl;
        al_destroy_display(display);
        return 0;
    }
    
    final = al_load_bitmap("statics/imagens/youwin.bmp");
    
    if(!final){
        cout << "Falha ao carregar a mensagem final!" << endl;
        al_destroy_display(display);
        return 0;
    }

    derrota = al_load_bitmap("statics/imagens/youlose.bmp");

    if(!derrota){
        cout << "Falha ao carregar a mensagem final!" << endl;
        al_destroy_display(display);
        return 0;
    }

    if(!al_install_audio()){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
    if(!al_init_acodec_addon()){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
    if (!al_reserve_samples(5)){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
    musica = al_load_audio_stream("statics/audios/PoisonTurtle.ogg",4,1024);
    if (!musica) {
        cout << "Falha na leitura da musica" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

    sombolinha = al_load_sample("statics/audios/som.ogg");
    if(!sombolinha){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
     somderrota = al_load_sample("statics/audios/player-died.ogg");
    if(!somderrota){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
     somvitoria = al_load_sample("statics/audios/level-win.ogg");
    if(!somvitoria){
        cout << "Falha!" << endl;
        al_destroy_display(display);
        return 0;
    }
    
    al_init_font_addon();
    if (!al_init_ttf_addon()){
        cout << "Falha ao inicializar add-on allegro_ttf" << endl;
        return -1;
    }

    fonte = al_load_font("statics/fontes/Starjedi.ttf", 24, 0);
    if (!fonte){
        al_destroy_display(display);
        cout << "Falha ao carregar fonte" << endl;
        return -1;
    }
    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }
    fantasminha1 = al_load_bitmap("clone.bmp"); //fantasma aleatorio
    if(!fantasminha1)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_draw_bitmap(fantasminha1,posfx1,posfy1,0);

    fantasminha2 = al_load_bitmap("darthmaul.bmp"); //fantasma aleatorio
    if(!fantasminha2)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_draw_bitmap(fantasminha2,posfx2,posfy2,0);

    fantasminha3 = al_load_bitmap("darthvader.bmp"); //fantasma inteligente
    if(!fantasminha3)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_draw_bitmap(fantasminha3,posfx3,posfy3,0);

    fantasminha4 = al_load_bitmap("mau.bmp"); //fantasma aleatorio
    if(!fantasminha4)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_draw_bitmap(fantasminha4,posfx4,posfy4,0);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;

    while(!sair)
    {
        
        if(pontuacao==259){
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(final,0,0,0);
            al_play_sample(somvitoria, 8.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            al_flip_display();
            al_rest(6.0);
            return 0;
        }
        if(((posy == posfy1) && (posx == posfx1)) || ((posy == posfy2) && (posx == posfx2)) || ((posy == posfy3) && (posx == posfx3)) || ((posy == posfy4) && (posx == posfx4))){
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(derrota,0,0,0);
            al_play_sample(somderrota, 8.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            al_flip_display();
            al_rest(6.0);
            return 0;
        }

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {

            /*fantasminhas*/
            //fantasma1
            if(MAPA[posfy1/20][posfx1/20]=='3' && MAPA[posfy1/20-1][posfx1/20] != '1'){
              k1--;
              posfy1 = k1*q;
            }
            if(MAPA[posfy1/20][posfx1/20]=='3' && MAPA[posfy1/20+1][posfx1/20] != '1'){
              k1--;
              posfy1 = k1*q;
            }
            if(MAPA[posfy1/20][posfx1/20]=='3' && MAPA[posfy1/20][posfx1/20-1] != '1'){
              l1--;
              posfx1 = l1*q;
            }
            if(MAPA[posfy1/20][posfx1/20]=='3' && MAPA[posfy1/20][posfx1/20+1] != '1'){
              l1++;
              posfx1 = l1*q;
            }
            if(movf1==2 && MAPA[posfy1/20][posfx1/20-1] == '1' || MAPA[posfy1/20][posfx1/20-1] == '3'){
              if(MAPA[posfy1/20-1][posfx1/20] != '1' && MAPA[posfy1/20+1][posfx1/20] != '1'){
                rand()%2 ? (movf1=0) : (movf1=1);
              }
              else(MAPA[posfy1/20-1][posfx1/20] != '1' ? (movf1=0) : (movf1=1));
            }
            if(movf1==3 && MAPA[posfy1/20][posfx1/20+1] == '1' || MAPA[posfy1/20][posfx1/20+1] == '3'){
              if(MAPA[posfy1/20-1][posfx1/20] != '1' && MAPA[posfy1/20+1][posfx1/20] != '1'){
                rand()%2 ? (movf1=0) : (movf1=1);
              }
              else(MAPA[posfy1/20-1][posfx1/20] != '1' ? (movf1=0) : (movf1=1));
            }
            if(movf1==1 && MAPA[posfy1/20+1][posfx1/20] == '1' || MAPA[posfy1/20+1][posfx1/20] == '3'){
              if(MAPA[posfy1/20][posfx1/20-1] != '1' && MAPA[posfy1/20][posfx1/20+1] != '1'){
                rand()%2 ? (movf1=2) : (movf1=3);
              }
              else(MAPA[posfy1/20][posfx1/20-1] != '1' ? (movf1=2) : (movf1=3));
            }
            if(movf1==0 && MAPA[posfy1/20-1][posfx1/20] == '1' || MAPA[posfy1/20-1][posfx1/20] == '3'){
              if(MAPA[posfy1/20][posfx1/20-1] != '1' && MAPA[posfy1/20][posfx1/20+1] != '1'){
                rand()%2 ? (movf1=2) : (movf1=3);
              }
              else(MAPA[posfy1/20][posfx1/20-1] != '1' ? (movf1=2) : (movf1=3));
            }
            if(movf1==0 && MAPA[posfy1/20-1][posfx1/20] != '1' && MAPA[posfy1/20-1][posfx1/20] != '3'){
              k1--;
              posfy1 = k1*q;
            }
            if(movf1==1 && MAPA[posfy1/20+1][posfx1/20] != '1' && MAPA[posfy1/20+1][posfx1/20] != '3'){
              k1++;
              posfy1 = k1*q;
            }
            if(movf1==2 && MAPA[posfy1/20][posfx1/20-1] != '1' && MAPA[posfy1/20][posfx1/20-1] != '3'){
              l1--;
              posfx1 = l1*q;
            }
            if(movf1==3 && MAPA[posfy1/20][posfx1/20+1] != '1' && MAPA[posfy1/20][posfx1/20+1] != '3'){
              l1++;
              posfx1 = l1*q;
            }
            //fantasma2
            if(MAPA[posfy2/20][posfx2/20]=='3' && MAPA[posfy2/20-1][posfx2/20] != '1'){
              k2--;
              posfy2 = k2*q;
            }
            if(MAPA[posfy2/20][posfx2/20]=='3' && MAPA[posfy2/20+1][posfx2/20] != '1'){
              k2--;
              posfy2 = k2*q;
            }
            if(MAPA[posfy2/20][posfx2/20]=='3' && MAPA[posfy2/20][posfx2/20-1] != '1'){
              l2--;
              posfx2 = l2*q;
            }
            if(MAPA[posfy2/20][posfx2/20]=='3' && MAPA[posfy2/20][posfx2/20+1] != '1'){
              l2++;
              posfx2 = l2*q;
            }
            if(movf2==2 && MAPA[posfy2/20][posfx2/20-1] == '1' || MAPA[posfy2/20][posfx2/20-1] == '3'){
              if(MAPA[posfy2/20-1][posfx2/20] != '1' && MAPA[posfy2/20+1][posfx2/20] != '1'){
                rand()%2 ? (movf2=0) : (movf2=1);
              }
              else(MAPA[posfy2/20-1][posfx2/20] != '1' ? (movf2=0) : (movf2=1));
            }
            if(movf2==3 && MAPA[posfy2/20][posfx2/20+1] == '1' || MAPA[posfy2/20][posfx2/20+1] == '3'){
              if(MAPA[posfy2/20-1][posfx2/20] != '1' && MAPA[posfy2/20+1][posfx2/20] != '1'){
                rand()%2 ? (movf2=0) : (movf2=1);
              }
              else(MAPA[posfy2/20-1][posfx2/20] != '1' ? (movf2=0) : (movf2=1));
            }
            if(movf2==1 && MAPA[posfy2/20+1][posfx2/20] == '1' || MAPA[posfy2/20+1][posfx2/20] == '3'){
              if(MAPA[posfy2/20][posfx2/20-1] != '1' && MAPA[posfy2/20][posfx2/20+1] != '1'){
                rand()%2 ? (movf2=2) : (movf2=3);
              }
              else(MAPA[posfy2/20][posfx2/20-1] != '1' ? (movf2=2) : (movf2=3));
            }
            if(movf2==0 && MAPA[posfy2/20-1][posfx2/20] == '1' || MAPA[posfy2/20-1][posfx2/20] == '3'){
              if(MAPA[posfy2/20][posfx2/20-1] != '1' && MAPA[posfy2/20][posfx2/20+1] != '1'){
                rand()%2 ? (movf2=2) : (movf2=3);
              }
              else(MAPA[posfy2/20][posfx2/20-1] != '1' ? (movf2=2) : (movf2=3));
            }
            if(movf2==0 && MAPA[posfy2/20-1][posfx2/20] != '1' && MAPA[posfy2/20-1][posfx2/20] != '3'){
              k2--;
              posfy2 = k2*q;
            }
            if(movf2==1 && MAPA[posfy2/20+1][posfx2/20] != '1' && MAPA[posfy2/20+1][posfx2/20] != '3'){
              k2++;
              posfy2 = k2*q;
            }
            if(movf2==2 && MAPA[posfy2/20][posfx2/20-1] != '1' && MAPA[posfy2/20][posfx2/20-1] != '3'){
              l2--;
              posfx2 = l2*q;
            }
            if(movf2==3 && MAPA[posfy2/20][posfx2/20+1] != '1' && MAPA[posfy2/20][posfx2/20+1] != '3'){
              l2++;
              posfx2 = l2*q;
            }
            //fantasma3
            if(MAPA[posfy1/20][posfx3/20]=='3' && MAPA[posfy3/20-1][posfx3/20] != '1'){
              k3--;
              posfy3 = k3*q;
            }
            if(MAPA[posfy3/20][posfx3/20]=='3' && MAPA[posfy3/20+1][posfx3/20] != '1'){
              k3--;
              posfy3 = k3*q;
            }
            if(MAPA[posfy3/20][posfx3/20]=='3' && MAPA[posfy3/20][posfx3/20-1] != '1'){
              l3--;
              posfx3 = l3*q;
            }
            if(MAPA[posfy3/20][posfx3/20]=='3' && MAPA[posfy3/20][posfx3/20+1] != '1'){
              l3++;
              posfx3 = l3*q;
            }
            if(posfx3>posx){
              movf3 = 2;
              if(movf3==2 && MAPA[posfy3/20][posfx3/20-1] != '1'){
                l3--;
                posfx3 = l3*q;
              }
            }
            if(posfx3<posx){
              movf3 = 3;
              if(movf3==3 && MAPA[posfy3/20][posfx3/20+1] != '1'){
                l3++;
                posfx3 = l3*q;
              }
            }
            if(posfy3>posy){
              movf3 = 0;
              if(movf3==0 && MAPA[posfy3/20-1][posfx3/20] != '1'){
                k3--;
                posfy3 = k3*q;
              }
            }
            if(posfy3<posy){
              movf3 = 1;
              if(movf3==1 && MAPA[posfy3/20+1][posfx3/20] != '1'){
                k3++;
                posfy3 = k3*q;
              }
            }
            if(movf3==2 && k3==10 && l3==0){
              l3=28;
              posfy3 = k3*q;
              posfx3 = l3*q;
            }
            if(movf1==3 && k3==10 && l3==28){
              k3=0;
              posfy3 = k3*q;
              posfx3 = l3*q; 
            }
            if(movf3==2 && k3==20 && l3==0){
              k3==28;
              posfy3=k3*q;
              posfx3=l3*q;
            }
            if(movf3==3 && k3==20 && l3==28){
              k3=0;
              posfy3 = k3*q;
              posfx3 = l3*q; 
            }
            //fantasma4
            if(MAPA[posfy4/20][posfx4/20]=='3' && MAPA[posfy4/20-1][posfx4/20] != '1'){
              k4--;
              posfy4 = k4*q;
            }
            if(MAPA[posfy4/20][posfx4/20]=='3' && MAPA[posfy4/20+1][posfx4/20] != '1'){
              k4--;
              posfy4 = k4*q;
            }
            if(MAPA[posfy4/20][posfx4/20]=='3' && MAPA[posfy4/20][posfx4/20-1] != '1'){
              l4--;
              posfx4 = l4*q;
            }
            if(MAPA[posfy4/20][posfx4/20]=='3' && MAPA[posfy4/20][posfx4/20+1] != '1'){
              l4++;
              posfx4 = l4*q;
            }
            if(movf4==2 && MAPA[posfy4/20][posfx4/20-1] == '1' || MAPA[posfy4/20][posfx4/20-1] == '3'){
              if(MAPA[posfy4/20-1][posfx4/20] != '1' && MAPA[posfy4/20+1][posfx4/20] != '1'){
                rand()%2 ? (movf4=0) : (movf4=1);
              }
              else(MAPA[posfy4/20-1][posfx4/20] != '1' ? (movf4=0) : (movf4=1));
            }
            if(movf4==3 && MAPA[posfy4/20][posfx4/20+1] == '1' || MAPA[posfy4/20][posfx4/20+1] == '3'){
              if(MAPA[posfy4/20-1][posfx4/20] != '1' && MAPA[posfy4/20+1][posfx4/20] != '1'){
                rand()%2 ? (movf4=0) : (movf4=1);
              }
              else(MAPA[posfy4/20-1][posfx4/20] != '1' ? (movf4=0) : (movf4=1));
            }
            if(movf4==1 && MAPA[posfy4/20+1][posfx4/20] == '1' || MAPA[posfy4/20+1][posfx4/20] == '3'){
              if(MAPA[posfy4/20][posfx4/20-1] != '1' && MAPA[posfy4/20][posfx4/20+1] != '1'){
                rand()%2 ? (movf4=2) : (movf4=3);
              }
              else(MAPA[posfy4/20][posfx4/20-1] != '1' ? (movf4=2) : (movf4=3));
            }
            if(movf4==0 && MAPA[posfy4/20-1][posfx4/20] == '1' || MAPA[posfy4/20-1][posfx4/20] == '3'){
              if(MAPA[posfy4/20][posfx4/20-1] != '1' && MAPA[posfy4/20][posfx4/20+1] != '1'){
                rand()%2 ? (movf4=2) : (movf4=3);
              }
              else(MAPA[posfy4/20][posfx4/20-1] != '1' ? (movf4=2) : (movf4=3));
            }
            if(movf4==0 && MAPA[posfy4/20-1][posfx4/20] != '1' && MAPA[posfy4/20-1][posfx4/20] != '3'){
              k4--;
              posfy4 = k4*q;
            }
            if(movf4==1 && MAPA[posfy4/20+1][posfx4/20] != '1' && MAPA[posfy4/20+1][posfx4/20] != '3'){
              k4++;
              posfy4 = k4*q;
            }
            if(movf4==2 && MAPA[posfy4/20][posfx4/20-1] != '1' && MAPA[posfy4/20][posfx4/20-1] != '3'){
              l4--;
              posfx4 = l4*q;
            }
            if(movf4==3 && MAPA[posfy4/20][posfx4/20+1] != '1' && MAPA[posfy4/20][posfx4/20+1] != '3'){
              l4++;
              posfx4 = l4*q;
            }
            /*********************************************/
            if(key[KEY_LEFT] && i==10 && j==0){
                j=28;
                posy = i*q;
                posx = j*q;
            }
            if(key[KEY_RIGHT] && i==10 && j==28){
                j=0;
                posy = i*q;
                posx = j*q;
            }
            if(key[KEY_LEFT] && i==20 && j==0){
                j=28;
                posy = i*q;
                posx = j*q;
            }
            if(key[KEY_RIGHT] && i==20 && j==28){
                j=0;
                posy = i*q;
                posx = j*q;
            }
            if(key[KEY_UP] && MAPA[i-1][j] != '1' || c==1 && MAPA[i-1][j] != '1')
            {
                c=1;
                b=0;
                d=0;
                e=0;
                if(MAPA[i-1][j]=='2'){
                    MAPA[i-1][j]='0';
                    pontuacao++;
                    al_play_sample(sombolinha, 4.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                i--;
                posy = i*q;
               if(MAPA[i-1][j] == '1'){
                 c=0;
               }
            }

            if(key[KEY_DOWN] && MAPA[i+1][j] != '1' || b==1 && MAPA[i+1][j] != '1')
            {
              b=1;
              c=0;
              e=0;
              d=0;
                if(MAPA[i+1][j]=='2'){
                    MAPA[i+1][j]='0';
                    pontuacao++;
                    al_play_sample(sombolinha, 4.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                i++;
                posy = i*q;
                if(MAPA[i+1][j] == '1'){
                  b=0;
                }
            }

            if(key[KEY_LEFT] && MAPA[i][j-1] != '1' || e==1 && MAPA[i][j+1] != '1')
            {
              e=1;
              c=0;
              d=0;
              b=0;
                if(MAPA[i][j-1]=='2'){
                    MAPA[i][j-1]='0';
                    pontuacao++;
                    al_play_sample(sombolinha, 4.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                j--;
                posx = j*q;
                if(MAPA[i][j-1] == '1'){
                  e=0;
                }
            }

            if(key[KEY_RIGHT] && MAPA[i][j+1] != '1' || d==1 && MAPA[i][j+1] != '1')
            {
              d=1;
              c=0;
              e=0;
              b=0;
                if(MAPA[i][j+1]=='2'){
                    MAPA[i][j+1]='0';
                    pontuacao++;
                    al_play_sample(sombolinha, 4.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                j++;
                posx = j*q;
                if(MAPA[i][j+1] == '1'){
                  d=0;
                }
            }

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                sair = true;
                break;
            }
        }
    
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_textf(fonte, al_map_rgb(255, 255, 0), SCREEN_H / 2, 640, ALLEGRO_ALIGN_CENTRE, "Pontuacao: %d", pontuacao);

            al_draw_bitmap(mapa,0,0,0);
            al_draw_bitmap(pacman,posx,posy,0);
            for(int k=0;k<32;k++){
                for(int l=0;l<29;l++){
                    if(MAPA[k][l]=='2'){
                      al_draw_bitmap(bolinha,(double)l*(q)+7,(double)k*(q)+7,0);
                    }
                }   
            }
            if(e==1 || b==1 || c==1){
            al_draw_bitmap(pacman,posx,posy,0);
            }
            if(d==1){
              al_draw_bitmap(pacmandir,posx,posy,0);
            }
            al_draw_bitmap(fantasminha1,posfx1,posfy1,0);
            al_draw_bitmap(fantasminha2,posfx2,posfy2,0);
            al_draw_bitmap(fantasminha3,posfx3,posfy3,0);
            al_draw_bitmap(fantasminha4,posfx4,posfy4,0);
            al_flip_display();
        }
    }

    al_destroy_bitmap(mapa);
    al_destroy_bitmap(pacman);
    al_destroy_bitmap(bolinha);
    al_destroy_bitmap(fantasminha1);
    al_destroy_bitmap(fantasminha2);
    al_destroy_bitmap(fantasminha3);
    al_destroy_bitmap(fantasminha4);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);


    return 0;
}
