#ifndef MAPDEFINITION_H
#define MAPDEFINITION_H

/**PATH TO THE IMAGE WITH THE MAP INTO RESOURCES**/
    #define MAP_IMAGE_PATH ":/mapImage.png"
/****/


/**COORDINATES**/

    /*Latitude at up left corner (pixel y=0)*/
    #define MAP_MIN_LATITUDE 40

    /*Longitude at up left corner (pixel x=0)*/
    #define MAP_MIN_LONGITUDE -14

    /*Latitude at bottom right corner (pixel y = height of the image)*/
    #define MAP_MAX_LATITUDE 34

    /*Longitude at bottom right corner (pixel x = width of the image)*/
    #define MAP_MAX_LONGITUDE -3

    /*Latitude Y-axis length in meters*/
    #define MAP_METRES_LATITUDE 667200

    /*Longitude X-axis length in meters (bottom)*/
    #define MAP_METRES_LONGITUDE_BOTTOM 1014520

    /*Longitude X-axis length in meters (top)*/
    #define MAP_METRES_LONGITUDE_TOP 936400



/****/


/**WAVE PROPAGATION SPEED**/

    /*Average velocity in m/s */
    #define PROPAGATION_SPEED 3600

/****/


/**COLORS**/

    /**EPICENTER**/
        /*RGB red value for the epicenter mark*/
        #define R_EPICENTER 255
        /*RGB green value for the epicenter mark*/
        #define G_EPICENTER 0
        /*RGB blue value for the epicenter mark*/
        #define B_EPICENTER 0
        /*Color transparency for the epicenter mark*/
        #define T_EPICENTER 255

    /**EXPANSION CIRCLES**/
        /*RGB red value for the first circle*/
        #define R_EPICENTER_FIRST_CIRCLE 50
        /*RGB green value for the first circle*/
        #define G_EPICENTER_FIRST_CIRCLE 50
        /*RGB blue value for the first circle*/
        #define B_EPICENTER_FIRST_CIRCLE 128
        /*Color transparency for the first circle*/
        #define T_EPICENTER_FIRST_CIRCLE 128

/****/

/**SIZES**/

    /**EPICENTER**/
        /*Epicenter mark radius*/
        #define RADIUS_EPICENTER 2

    /**STATIONS**/
        /*X-axis lenght for the station triangle*/
        #define STATION_SIZE_X 5
        /*Y-axis lenght for the station triangle*/
        #define STATION_SIZE_Y 10

/****/

#endif // MAPDEFINITION_H
