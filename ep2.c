#include <stdio.h>

#define PI 3.14159
#define RAIO_AP 200
#define RAIO_ZA 2000 /* zona de alerta */
#define DELTA_ALARME 60
#define EPS_COS 0.000001 /* precisao para calculo do cosseno */
#define EPS 0.01 /* precisao para valores envolvendo metros */

int iguais(double x, double y);
double cosseno(double theta, double epsilon);
int localiza (double xi, double yi, double div, double xj, double yj, double djv, double xk, double yk, double dkv, double *xv, double *yv);
double velocidade(double x0, double y0, double x1, double y1, double deltaT);
int intercepta(double x0, double y0, double x1, double y1, double *x, double *y);
double raiz(double x, double epsilon);
double aoq(double);
void troca(double *, double *);
double distancia(double, double, double, double);

int main(){
    FILE *arq;
    char filename[256];

    int casos_teste, v, flag;

    int id_i0; double x_i0, y_i0, dv_i0, h_i0, theta_i0;
    int id_j0; double x_j0, y_j0, dv_j0, h_j0, theta_j0;
    int id_k0; double x_k0, y_k0, dv_k0, h_k0, theta_k0;

    int id_i1; double x_i1, y_i1, dv_i1, h_i1, theta_i1;
    int id_j1; double x_j1, y_j1, dv_j1, h_j1, theta_j1;
    int id_k1; double x_k1, y_k1, dv_k1, h_k1, theta_k1;

    double x0, x1, y0, y1, x_inter, y_inter;
    double vel_media, deltaS, deltaT;
    double dist_origem;

    printf("Programa-demo para o Sistema de Monitoramento\n\n\n\n");

    /* Leitura do arquivo */
    printf("Digite o nome do arquivo com os dados a serem processados: ");
    scanf("%s", filename);
    arq = fopen(filename, "r");
    if(arq == NULL){
        printf("Nao consegui abrir o arquivo %s.\n", filename);
        return 0;
    }

    printf("\n");
    fscanf(arq, "%d", &casos_teste);
    printf("Numero de casos a serem analisados: %d\n\n\n\n", casos_teste);

    /* Inicio do codigo em si */
    for(; casos_teste > 0; casos_teste--){

        flag = 1;

        /* Leitura dos inputs */
        fscanf(arq, "%d", &v);
        
        fscanf(arq, "%d %lf %lf %lf %lf", &id_i0, &x_i0, &y_i0, &h_i0, &theta_i0);
        fscanf(arq, "%d %lf %lf %lf %lf", &id_j0, &x_j0, &y_j0, &h_j0, &theta_j0);
        fscanf(arq, "%d %lf %lf %lf %lf", &id_k0, &x_k0, &y_k0, &h_k0, &theta_k0);

        fscanf(arq, "%lf", &deltaT);

        fscanf(arq, "%d %lf %lf %lf %lf", &id_i1, &x_i1, &y_i1, &h_i1, &theta_i1);
        fscanf(arq, "%d %lf %lf %lf %lf", &id_j1, &x_j1, &y_j1, &h_j1, &theta_j1);
        fscanf(arq, "%d %lf %lf %lf %lf", &id_k1, &x_k1, &y_k1, &h_k1, &theta_k1);

        /* Sinais relativos a posicao previa */
        dv_i0 = h_i0 * cosseno(theta_i0, EPS_COS);
        dv_j0 = h_j0 * cosseno(theta_j0, EPS_COS);
        dv_k0 = h_k0 * cosseno(theta_k0, EPS_COS);

        printf("IDENTIFICACAO: veiculo %d\n\n", v);
        printf("Antenas na posicao previa\n");
        printf(" id | posicao | H (m) | theta (graus) | distancia (m)\n");
        printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n", id_i0, x_i0, y_i0, h_i0, theta_i0, dv_i0);
        printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n", id_j0, x_j0, y_j0, h_j0, theta_j0, dv_j0);
        printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n\n", id_k0, x_k0, y_k0, h_k0, theta_k0, dv_k0);

        if(localiza(x_i0, y_i0, dv_i0, x_j0, y_j0, dv_j0, x_k0, y_k0, dv_k0, &x0, &y0))
            printf("Localizacao previa: (%8.2f,%8.2f)\n\n", x0, y0);
        else{
            printf("Nao foi possivel calcular a localizacao inicial do veiculo %d.\n\n", v);
            printf("Nao foi possivel determinar a situacao do veiculo %d\n\n", v);
            flag = 0;
        }       

        if(flag){
            /* Intervalo T */
            printf("Intervalo de tempo: %.2f segundos\n\n", deltaT);

            /* Sinais relativos a posicao atual */
            dv_i1 = h_i1 * cosseno(theta_i1, EPS_COS);
            dv_j1 = h_j1 * cosseno(theta_j1, EPS_COS);
            dv_k1 = h_k1 * cosseno(theta_k1, EPS_COS);

            printf("Antenas na posicao atual\n");
            printf(" id | posicao | H (m) | theta (graus) | distancia (m)\n");
            printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n", id_i1, x_i1, y_i1, h_i1, theta_i1, dv_i1);
            printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n", id_j1, x_j1, y_j1, h_j1, theta_j1, dv_j1);
            printf("  %d | (%8.2f, %8.2f) |%9.2f |%9.2f |%9.2f\n\n", id_k1, x_k1, y_k1, h_k1, theta_k1, dv_k1);

            if(localiza(x_i1, y_i1, dv_i1, x_j1, y_j1, dv_j1, x_k1, y_k1, dv_k1, &x1, &y1))
                printf("Localizacao atual: (%8.2f,%8.2f)\n\n", x1, y1);
            else{
                printf("Nao foi possivel calcular a localizacao atual do veiculo %d.\n\n", v);
                printf("Nao foi possivel determinar a situacao do veiculo %d\n\n", v);
                flag = 0;
            }
        }

        if(flag){
            /* Situacao do veiculo */
            deltaS = distancia(x0, y0, x1, y1);
            printf("Distancia percorrida: %0.2f m\n", deltaS);

            vel_media = velocidade(x0, y0, x1, y1, deltaT);
            printf("Velocidade: %0.2f m/s\n\n", vel_media);

            dist_origem = distancia(0, 0, x1, y1);
            printf("Distancia da origem: %0.2f\n", dist_origem);

            /* Veiculo na AP */
            if(dist_origem <= RAIO_AP){
                if(iguais(vel_media, 0.0)){
                    printf("Veiculo estacionado na AP\n\n");
                    printf("*************************************\n");
                    printf("  ALARME, ALARME, ALARME, ALARME !!\n");
                    printf("       Veiculo esta na AP !\n");
                    printf("*************************************\n\n");                  
                }
                else{
                    printf("Veiculo em movimento na AP\n");
                    printf("*************************************\n");
                    printf("  ALARME, ALARME, ALARME, ALARME !!\n");
                    printf("       Veiculo esta na AP !\n");
                    printf("*************************************\n\n");
                }
            }

            /* Veiculo dentro da Zona de Alerta */
            else if(dist_origem <= RAIO_ZA){
                if(iguais(vel_media, 0.0))
                    printf("Veiculo estacionado na ZONA de ALERTA\n\n");               
                else{
                    printf("Veiculo em movimento na ZONA DE ALERTA\n\n");

                    /*arrumar as variaveis disso aqui*/
                    if(intercepta(x0, y0, x1, y1, &x_inter, &y_inter)){
                        printf("Trajetoria INTERCEPTARA a AP.\n");
                        printf("Distancia atual da AP e de %0.2f metros.\n", distancia(x_inter, y_inter, x1, y1));
                        printf("A Interceptacao ocorrera em %0.2f segundos,\n", distancia(x1, y1, x_inter, y_inter) / vel_media);
                        printf("na coordenada (%8.2f, %8.2f).\n\n", x_inter, y_inter);
                        if((distancia(x_inter, y_inter, x1, y1) / vel_media) <= DELTA_ALARME){
                            printf("*****************************************\n");
                            printf("  ALARME, ALARME, ALARME, ALARME !!\n");
                            printf("      Invasao iminente !     \n");
                            printf("*****************************************\n\n");
                            
                        }
                    }
                    else
                        printf("Veiculo NAO INTERCEPTARA a AP.\n\n");
                }
            }

            /* Veiculo fora da Zona de Alerta */
            else{
                if(iguais(vel_media, 0.0))
                    printf("Veiculo estacionado FORA da zona de alerta\n\n\n\n");
                else
                    printf("Veiculo em movimento FORA da zona de alerta\n\n\n\n");
            }
   
        }

        printf("\n\n\n");
  
    }

    fclose(arq);

    return 0;
}

int iguais(double x, double y){
    if(x-y < EPS && y-x < EPS)
        return 1;
    else
        return 0;
}

double raiz(double x, double epsilon){
    double raiz, raiz_ant, raiz_post;
    int flag = 1;
    raiz_ant = x;
    while(flag){
        raiz = (raiz_ant + (x / raiz_ant)) / 2;
        raiz_ant = raiz;
        raiz_post = (raiz + (x / raiz)) / 2;
        if (raiz - raiz_post < epsilon)
            flag = 0;
    }
    return raiz_post;
}

double aoq(double n){
    return (n * n);
}

void troca(double *a, double *b) {
    double tmp; tmp = *a; *a = *b; *b = tmp;
}

double cosseno(double theta, double epsilon){    
    double soma = 0, termo, numerador = 1;
    int denominador = 1, k = 0, flag = 1;
    theta *= PI / 180.0;

    while(flag){       
        if(k == 0)
            termo = 1;
        else{
            numerador *= theta * theta;
            denominador *= k * (k - 1);
            termo = numerador / denominador;
        }

        if(termo < epsilon){
            if((k / 2) % 2 != 0)
                termo *= -1;
            soma += termo;
            flag = 0;
        }
        else{
            if((k / 2) % 2 != 0)
                termo *= -1;
            soma += termo;
        }

        k += 2;
    }
    return soma;
}

double distancia(double x0, double y0, double x1, double y1){
    if(iguais(x0, x1) && iguais(y0, y1))
        return 0;
    else  
        return raiz(aoq(x1 - x0) + aoq(y1 - y0), EPS); 
}

double velocidade(double x0, double y0, double x1, double y1, double deltaT){
    if(iguais(x0, x1) && iguais(y0, y1))
        return 0.0;
    else
        return distancia(x0, y0, x1, y1) / deltaT;
}

int localiza(double xi, double yi, double div, double xj, double yj, double djv, double xk, double yk, double dkv, double *xv, double *yv){
    double pij, pik, qij, qik;

    if( iguais(xi, xj) ){
        if(iguais(xj, xk)){
            return 0;
        }
        else{
            troca(&xi, &xk); troca(&yi, &yk); troca(&div, &dkv);
        }
    }
    else{
        if(iguais(xi, xk)){
            troca(&xi, &xj); troca(&yi, &yj); troca(&div, &djv);
        }
            
    }

    pij = (aoq(xi) - aoq(xj) + aoq(yi) - aoq(yj) - aoq(div) + aoq(djv)) / (2 * (xi - xj));
    qij = (yj - yi) / (xi - xj);

    pik = (aoq(xi) - aoq(xk) + aoq(yi) - aoq(yk) - aoq(div) + aoq(dkv)) / (2 * (xi - xk));
    qik = (yk - yi) / (xi - xk);

    *yv = (pik - pij) / (qij - qik);
    *xv = pij + qij * *yv;
    
    return 1;
}

int intercepta(double x0, double y0, double x1, double y1, double *x, double *y){
    double x0_mod;
    double x0_alt, y0_alt, x1_alt, y1_alt, dist0, dist1;
    double a, b, delta;

    if(distancia(x1, y1, 0, 0) > distancia(x0, y0, 0, 0))
        return 0;

    if(iguais(x0, x1)){
        x0_mod = x0;
        if(x0_mod < 0)
            x0_mod = - x0_mod;
        
        if(iguais(x0_mod, RAIO_AP)){
            *x = RAIO_AP; *y = 0.0;
            return 1;
        }

        if(x0_mod < RAIO_AP){
            y0_alt = raiz(aoq(RAIO_AP) - aoq(x0), EPS);
            y1_alt = - y0_alt;
        
            if(y1 >= 0.0){
                if(y1 - y0_alt < y1 - y1_alt){  
                    *x = x0; *y = y0_alt;
                }
                else{
                    *x = x0; *y = y1_alt;
                }
            }
            else{
                if(y1 - y0_alt > y1 - y1_alt){ 
                    *x = x0; *y = y0_alt;
                }
                else{
                    *x = x0; *y = y1_alt;
                }
            }

            return 1;
        }

        return 0;
    }

    else{
        a = (y1 - y0) / (x1 - x0);
        b = y0 - (a * x0);
        delta = 4 * (aoq(a) * aoq(RAIO_AP) + aoq(RAIO_AP) - aoq(b));

        if(delta < 0.0)
            return 0;

        else if(iguais(delta, 0.0)){
            *x = ((-2) * a * b) / (2 * (aoq(a) + 1));
            *y = *x * a + b;
        }

        else{        
            x0_alt = (raiz(delta, EPS) - 2 * a * b) / (2 * aoq(a) + 2);
            y0_alt = x0_alt * a + b;
            
            x1_alt = (- raiz(delta, EPS) - 2 * a * b) / (2 * aoq(a) + 2);
            y1_alt = x1_alt * a + b;
            
            dist0 = distancia(x1, y1, x0_alt, y0_alt);
            dist1 = distancia(x1, y1, x1_alt, y1_alt);

            if (dist0 < dist1){
                *x = x0_alt; *y = y0_alt;
            }
            else{
                *x = x1_alt; *y = y1_alt;
            }
        }
    }

    return 1;
}