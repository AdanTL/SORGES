#ifndef MAPDEFINITION_H
#define MAPDEFINITION_H

/**path de la imagen del mapa**/
    #define MAP_IMAGE_PATH ":/mapacortado.png"
/****/


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

/****/


/**VELOCIDAD PROPAGACION**/

    /*Velocidad media de onda S (m/s)*/
    #define EXPANSION_SPEED 3600

/****/


/**COLORES**/

    /**EPICENTRO**/
        /*Valor de rojo en el epicentro*/
        #define R_EPICENTER 255
        /*Valor verde en el epicentro*/
        #define G_EPICENTER 0
        /*Valor azul en el epicentro*/
        #define B_EPICENTER 0
        /*Valor de transparencia del color en el epicentro*/
        #define T_EPICENTER 255

    /**CIRCULOS EXPANSION**/
        /*Valor de rojo en el primer circulo de expansion*/
        #define R_EPICENTER_FIRST_CIRCLE 50
        /*Valor de verde en el primer circulo de expansion*/
        #define G_EPICENTER_FIRST_CIRCLE 50
        /*Valor de azul en el primer circulo de expasion*/
        #define B_EPICENTER_FIRST_CIRCLE 128
        /*Valor de transparencia del color en el primer circulo de expansion*/
        #define T_EPICENTER_FIRST_CIRCLE 128

/****/

/**TAMAÑOS**/

    /**EPICENTRO**/
        /*Valor de radio del epicentro*/
        #define RADIUS_EPICENTER 2

    /**ESTACIONES**/
        /*Tamaño en el eje X del triangulo de la estacion (finalmente este valor es x2)*/
        #define STATION_SIZE_X 5
        /*Tamaño en el eje Y del triangulo de la estacion (finalmente este valor es x2)*/
        #define STATION_SIZE_Y 10

/****/

#endif // MAPDEFINITION_H
