#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "math.h"



/*======== void add_box() ==========
   Inputs:   struct matrix * edges
            double x
            double y
            double z
            double width
            double height
            double depth

   add the points for a rectagular prism whose
   upper-left-front corner is (x, y, z) with width,
   height and depth dimensions.
   ====================*/
void add_box( struct matrix * edges,
              double x, double y, double z,
              double w, double h, double d) {

	add_edge(edges, x, y, z, x + w, y, z);
	add_edge(edges, x, y, z, x, y - h, z);
	add_edge(edges, x, y, z, x, y, z - d);

	add_edge(edges, x + w, y, z, x + w, y - h, z);
	add_edge(edges, x + w, y, z, x + w, y, z - d);

	add_edge(edges, x, y - h, z, x, y - h, z - d);
	add_edge(edges, x, y - h, z, x + w, y - h, z);

	add_edge(edges, x + w, y - h, z - d, x + w, y - h, z);
	add_edge(edges, x + w, y - h, z - d, x + w, y, z - d);
	add_edge(edges, x + w, y - h, z - d, x, y - h, z - d);

	add_edge(edges, x + w, y, z - d, x, y, z - d);

	add_edge(edges, x, y, z - d, x, y - h, z - d);

}

/*======== void add_sphere() ==========
   Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step

   adds all the points for a sphere with center (cx, cy, cz)
   and radius r using step points per circle/semicircle.

   Since edges are drawn using 2 points, add each point twice,
   or add each point and then another point 1 pixel away.

   should call generate_sphere to create the necessary points
   ====================*/
void add_sphere( struct matrix * edges,
                 double cx, double cy, double cz,
                 double r, int step ) {

    struct matrix * points = generate_sphere(cx, cy, cz, r, step);

    for(int i = 0; i < step * step; i++){

        double x = points->m[0][i];
        double y = points->m[1][i];
        double z = points->m[2][i];

        add_edge(edges, x, y, z, x+1, y+1, z+1);

    }


}

/*======== void generate_sphere() ==========
   Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
   Returns: Generates all the points along the surface
           of a sphere with center (cx, cy, cz) and
           radius r using step points per circle/semicircle.
           Returns a matrix of those points
   ====================*/
struct matrix * generate_sphere(double cx, double cy, double cz,
                                double r, int step ) {


    struct matrix * sphere = new_matrix(step * step, 4);

    for(int i = 1; i <= step; i++){
        double phi = (double) i / step;

        for(int j = 1; j <= step; j++){
            double theta = (double) j / step;

            double x = r * cos(M_PI * theta) + cx;
            double y = r * sin(M_PI * theta) * cos(2 * M_PI * phi) + cy;
            double z = r * sin(M_PI * theta) * sin(2 * M_PI * phi) + cz;

            add_point(sphere, x, y, z);
        }

    }

    return sphere;
}

/*======== void add_torus() ==========
   Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r1
            double r2
            double step
   Returns:

   adds all the points required for a torus with center (cx, cy, cz),
   circle radius r1 and torus radius r2 using step points per circle.

   should call generate_torus to create the necessary points
   ====================*/
void add_torus( struct matrix * edges,
                double cx, double cy, double cz,
                double r, double R, int step ) {

    struct matrix * points = generate_torus(cx, cy, cz, r, R, step);

    for(int i = 0; i < step * step; i++){

        double x = points->m[0][i];
        double y = points->m[1][i];
        double z = points->m[2][i];

        add_edge(edges, x, y, z, x+1, y+1, z+1);

    }

}

/*======== void generate_torus() ==========
   Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
   Returns: Generates all the points along the surface
           of a torus with center (cx, cy, cz),
           circle radius r1 and torus radius r2 using
           step points per circle.
           Returns a matrix of those points
   ====================*/
struct matrix * generate_torus( double cx, double cy, double cz,
                                double r, double R, int step ) {

    struct matrix * torus = new_matrix(step * step, 4);

    for(int i = 1; i <= step; i++){
        double phi = (double) i / step;

        for(int j = 1; j <= step; j++){
            double theta = (double) j / step;

            double x = cos(2 * M_PI * phi) * (r * cos(2 * M_PI * theta) + R) + cx;
            double y = r * sin(2 * M_PI * theta) + cy;
            double z = -sin(2 * M_PI * phi) * (r * cos(2 * M_PI * theta) + R) + cz;

            add_point(torus, x, y, z);
        }

    }

    return torus;
}

/*======== void add_circle() ==========
   Inputs:   struct matrix * edges
            double cx
            double cy
            double r
            double step

   Adds the circle at (cx, cy) with radius r to edges
   ====================*/
void add_circle( struct matrix *edges,
                 double cx, double cy, double cz,
                 double r, int step ) {
	double x0, y0, x1, y1, t;
	int i;

	x0 = r + cx;
	y0 = cy;
	for (i=1; i<=step; i++) {
		t = (double)i/step;
		x1 = r * cos(2 * M_PI * t) + cx;
		y1 = r * sin(2 * M_PI * t) + cy;

		add_edge(edges, x0, y0, cz, x1, y1, cz);
		x0 = x1;
		y0 = y1;
	}
}


/*======== void add_curve() ==========
   Inputs:   struct matrix *edges
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type

   Adds the curve bounded by the 4 points passsed as parameters
   of type specified in type (see matrix.h for curve type constants)
   to the matrix edges
   ====================*/
void add_curve( struct matrix *edges,
                double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                int step, int type ) {
	double t, x, y;
	int i;
	struct matrix *xcoefs;
	struct matrix *ycoefs;

	xcoefs = generate_curve_coefs(x0, x1, x2, x3, type);
	ycoefs = generate_curve_coefs(y0, y1, y2, y3, type);

	/* print_matrix(xcoefs); */
	/* printf("\n"); */
	/* print_matrix(ycoefs); */

	for (i=1; i<=step; i++) {
		t = (double)i/step;
		x = xcoefs->m[0][0] *t*t*t + xcoefs->m[1][0] *t*t+
		    xcoefs->m[2][0] *t + xcoefs->m[3][0];
		y = ycoefs->m[0][0] *t*t*t + ycoefs->m[1][0] *t*t+
		    ycoefs->m[2][0] *t + ycoefs->m[3][0];

		add_edge(edges, x0, y0, 0, x, y, 0);
		x0 = x;
		y0 = y;
	}

	free_matrix(xcoefs);
	free_matrix(ycoefs);
}



/*======== void add_point() ==========
   Inputs:   struct matrix * points
         int x
         int y
         int z
   Returns:
   adds point (x, y, z) to points and increment points.lastcol
   if points is full, should call grow on points
   ====================*/
void add_point( struct matrix * points, double x, double y, double z) {

	if ( points->lastcol == points->cols )
		grow_matrix( points, points->lastcol + 100 );

	points->m[0][ points->lastcol ] = x;
	points->m[1][ points->lastcol ] = y;
	points->m[2][ points->lastcol ] = z;
	points->m[3][ points->lastcol ] = 1;
	points->lastcol++;
} //end add_point

/*======== void add_edge() ==========
   Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
   Returns:
   add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
   should use add_point
   ====================*/
void add_edge( struct matrix * points,
               double x0, double y0, double z0,
               double x1, double y1, double z1) {
	add_point( points, x0, y0, z0 );
	add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
   Inputs:   struct matrix * points
         screen s
         color c
   Returns:
   Go through points 2 at a time and call draw_line to add that line
   to the screen
   ====================*/
void draw_lines( struct matrix * points, screen s, color c) {

	if ( points->lastcol < 2 ) {
		printf("Need at least 2 points to draw a line!\n");
		return;
	}

	int point;
	for (point=0; point < points->lastcol-1; point+=2)
		draw_line( points->m[0][point],
		           points->m[1][point],
		           points->m[0][point+1],
		           points->m[1][point+1],
		           s, c);
}// end draw_lines









void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {

	int x, y, d, A, B;
	//swap points if going right -> left
	int xt, yt;
	if (x0 > x1) {
		xt = x0;
		yt = y0;
		x0 = x1;
		y0 = y1;
		x1 = xt;
		y1 = yt;
	}

	x = x0;
	y = y0;
	A = 2 * (y1 - y0);
	B = -2 * (x1 - x0);

	//octants 1 and 8
	if ( abs(x1 - x0) >= abs(y1 - y0) ) {

		//octant 1
		if ( A > 0 ) {

			d = A + B/2;
			while ( x < x1 ) {
				plot( s, c, x, y );
				if ( d > 0 ) {
					y+= 1;
					d+= B;
				}
				x++;
				d+= A;
			} //end octant 1 while
			plot( s, c, x1, y1 );
		} //end octant 1

		//octant 8
		else {
			d = A - B/2;

			while ( x < x1 ) {
				//printf("(%d, %d)\n", x, y);
				plot( s, c, x, y );
				if ( d < 0 ) {
					y-= 1;
					d-= B;
				}
				x++;
				d+= A;
			} //end octant 8 while
			plot( s, c, x1, y1 );
		} //end octant 8
	}//end octants 1 and 8

	//octants 2 and 7
	else {

		//octant 2
		if ( A > 0 ) {
			d = A/2 + B;

			while ( y < y1 ) {
				plot( s, c, x, y );
				if ( d < 0 ) {
					x+= 1;
					d+= A;
				}
				y++;
				d+= B;
			} //end octant 2 while
			plot( s, c, x1, y1 );
		} //end octant 2

		//octant 7
		else {
			d = A/2 - B;

			while ( y > y1 ) {
				plot( s, c, x, y );
				if ( d > 0 ) {
					x+= 1;
					d+= A;
				}
				y--;
				d-= B;
			} //end octant 7 while
			plot( s, c, x1, y1 );
		} //end octant 7
	}//end octants 2 and 7
} //end draw_line
