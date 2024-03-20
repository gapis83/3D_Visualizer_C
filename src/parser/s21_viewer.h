#ifndef SRC_S21_VIEWER_H_
#define SRC_S21_VIEWER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846

typedef struct Peak_List {
  int counter_vertex;
  double *coordinates;
} Vertex;

typedef struct Facet_List {
  int counter_facets;
  int counter_lines;
  unsigned *lines;
} Facet;

typedef struct Move {
  double dx;
  double dy;
  double dz;
} Step;

typedef struct Turn_image {
  double rx;
  double ry;
  double rz;
} Rotation;

int parser(char *file_name, Vertex *vertexes, Facet *facets);
int count_param(FILE *obj, Vertex *vertexes, Facet *facets);
int find_value(FILE *obj, Vertex *vertexes, Facet *facets, int *ponter,
               int *line);
int pars_v_string(char *str, Vertex *vertexes, int *v_pointer, int length,
                  double *min_v, double *max_v);
int pars_f_string(char *str, Facet *facets, int *line, int length);
int add_memory(Vertex *vertexes, Facet *facets);
int count_numbers(char *f_string);
void scaling_image(Vertex *vertexes, double scaling);
void displace(Vertex *vertexes, Step step);
void rotation_model(Vertex *vertexes, Rotation rotation);
double grad_to_rad(double grad);
void values_after_rotation(Vertex *vertexes, double angle_rotation, int axis_1,
                           int axis_2);

#endif  // SRC_S21_VIEWER_H_
