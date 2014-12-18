#ifndef MAPDEFINITION_H
#define MAPDEFINITION_H

/**path de la imagen del mapa**/
#define MAP_IMAGE_PATH ":/mapacortado.png"

/**COORDENADAS**/

/*latitud en la esquina superior izquierda (pixel y=0)*/
#define MAP_MIN_LATITUDE 38

/*longitud en la esquina superior izquierda (pixel x=0)*/
#define MAP_MIN_LONGITUDE -14

/*latitud en la esquina inferior derecha (máxima)*/
#define MAP_MAX_LATITUDE 34

/*longitud en la esquina inferior derecha (máxima)*/
#define MAP_MAX_LONGITUDE -3

/*longitud en metros del eje latitudinal*/
#define MAP_METRES_LATITUDE 445210

/*longitud en metros del eje longitudinal*/
#define MAP_METRES_LONGITUDE 1014520

/*Velocidad media de onda S (m/s)*/
#define SPEED_EXPANSION 3000


/*Valor de rojo en el epicentro*/
#define R_EPICENTER 255
/*Valor verde en el epicentro*/
#define G_EPICENTER 0
/*Valor azul en el epicentro*/
#define B_EPICENTER 0
/*Valor de transparencia del color en el epicentro*/
#define T_EPICENTER 255
/*Valor de radio del epicentro*/
#define RADIUS_EPICENTER 2


/*Valor de rojo en el primer circulo de expansion*/
#define R_EPICENTER_FIRST_CIRCLE 50
/*Valor de verde en el primer circulo de expansion*/
#define G_EPICENTER_FIRST_CIRCLE 50
/*Valor de azul en el primer circulo de expasion*/
#define B_EPICENTER_FIRST_CIRCLE 128
/*Valor de transparencia del color en el primer circulo de expansion*/
#define T_EPICENTER_FIRST_CIRCLE 128

#endif // MAPDEFINITION_H
