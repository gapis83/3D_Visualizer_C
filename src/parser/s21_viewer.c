#include "s21_viewer.h"

int parser(char *file_name, Vertex *vertexes, Facet *facets) {
  int error = 0;
  if (file_name == NULL) {
    error = 1;
    vertexes->counter_vertex = -1;
  } else {
    FILE *mf = NULL;
    mf = fopen(file_name, "r");
    if (!mf) {
      error = 1;
      vertexes->counter_vertex = -1;
    }
    if (!error) {
      if (mf == NULL) {
        error = 1;
      } else {
        error = count_param(mf, vertexes, facets);
      }
      if (!error) error = add_memory(vertexes, facets);
      rewind(mf);
      int vertex_matrix = 3;
      int facet_matrix = 0;
      if (!error)
        error = find_value(mf, vertexes, facets, &vertex_matrix, &facet_matrix);
      if (fclose(mf) == EOF) error = 3;
    }
  }
  if (error > 0 && vertexes->counter_vertex > 0) {
    vertexes->counter_vertex = -1;
    facets->counter_facets = 0;
    facets->counter_lines = 0;
    free(vertexes->coordinates);
    free(facets->lines);
  }
  return error;
}

int count_param(FILE *obj, Vertex *vertexes, Facet *facets) {
  char str[200];
  char *estr;
  int error = 0;
  int counter_vertex = 0;
  int counter_facets = 0;
  int counter_lines = 0;
  int stop = 0;
  while (!feof(obj) && stop == 0) {
    estr = fgets(str, sizeof(str), obj);
    if (estr != NULL) {
      if (!error) {
        if (str[0] == 'v' && str[1] == ' ') {
          counter_vertex++;
        } else if (str[0] == 'f' && str[1] == ' ') {
          counter_facets++;
          counter_lines += count_numbers(str);
        }
      }
    } else {
      if (feof(obj) != 0) {
        stop = 1;
      } else {
        error = 2; /* Ошибка чтения из файла */
        stop = 1;
      }
    }
  }
  if (counter_vertex < 3 || counter_facets < 1) {
    error = 4;
  } else {
    vertexes->counter_vertex = counter_vertex;
    facets->counter_facets = counter_facets;
    facets->counter_lines = counter_lines;
  }
  return error;
}

int find_value(FILE *obj, Vertex *vertexes, Facet *facets, int *v_pointer,
               int *line) {
  char str[500];
  char *estr;
  int error = 0;
  int stop = 0;
  double min_vertex = 0;
  double max_vertex = 0;
  while (!feof(obj) && stop == 0) {
    estr = fgets(str, sizeof(str), obj);
    if (estr == NULL) {
      if (feof(obj) != 0) {
        stop = 1;
      } else {
        error = 2;
        stop = 1;
      }
    }
    if ((str[0] == 'v' || str[0] == 'f') && str[1] == ' ') {
      int length = strlen(str);
      if (str[0] == 'v') {
        error = pars_v_string(str, vertexes, v_pointer, length, &min_vertex,
                              &max_vertex);
      } else if (str[0] == 'f') {
        error = pars_f_string(str, facets, line, length);
      }
    }
    if (error > 0) stop = 1;
  }
  double amplitude = max_vertex - min_vertex;
  if (amplitude > 0.01) {
    memset(vertexes->coordinates, 0, 1);
    // double scaling_factor = amplitude / 1.5;
    double scaling_factor = 1;
    scaling_image(vertexes, scaling_factor);
  } else {
    error = 1;
  }
  return error;
}

int pars_v_string(char *str, Vertex *vertexes, int *v_pointer, int length,
                  double *min_v, double *max_v) {
  int error = 0;
  int n = *v_pointer;
  if (length < 6) {
    error = 6;
  } else {
    char sep[3] = "v ";
    int space = 0;
    char *istr;
    double max = *max_v;
    double min = *min_v;
    istr = strtok(str, sep);
    char **enstr = &istr;
    while (istr != NULL) {
      double value = strtod(istr, enstr);
      space++;
      istr = strtok(NULL, sep);
      if (space < 4) {
        if (value > max) max = value;
        if (value < min) min = value;
        vertexes->coordinates[n] = value;
        n++;
      }
    }
    *v_pointer = n;
    *min_v = min;
    *max_v = max;
  }
  return error;
}

int pars_f_string(char *str, Facet *facets, int *line, int length) {
  int error = 0;
  char sep[3] = "f ";
  unsigned long first_vertex;
  int point = *line;
  if (length < 6) {
    error = 6;
  } else {
    char *istr;
    char *endptr;
    istr = strtok(str, sep);
    first_vertex = strtoul(istr, &endptr, 10);
    int count = 0;
    while (istr != NULL) {
      unsigned vertex_num = (unsigned)strtoul(istr, &endptr, 10);
      count++;
      istr = strtok(NULL, sep);
      facets->lines[point] = vertex_num;
      point++;
      if (count != 1) {
        facets->lines[point] = vertex_num;
        point++;
      }
    }
    facets->lines[point] = first_vertex;
    *line = point + 1;
  }
  return error;
}

int add_memory(Vertex *vertexes, Facet *facets) {
  int error = 0;
  int vertex_memory = (vertexes->counter_vertex + 3) * 3;
  facets->counter_lines += 1;
  int facet_memory = facets->counter_lines * 2;
  vertexes->coordinates = (double *)malloc(vertex_memory * sizeof(double));
  if (vertexes->coordinates == NULL) {
    error = 1;
  } else {
    vertexes->coordinates[0] = 0;
    vertexes->coordinates[1] = 0;
    vertexes->coordinates[2] = 0;
    facets->lines = (unsigned *)malloc(facet_memory * sizeof(unsigned));
  }
  if (facets->lines == NULL) {
    error = 1;
    free(vertexes->coordinates);
  } else {
    facets->lines[0] = 0;
  }
  if (error) vertexes->counter_vertex = -1;
  return error;
}

int count_numbers(char *f_string) {
  int res = 0;
  int leng = strlen(f_string);
  for (int i = 0; i < leng; i++) {
    if (f_string[i - 1] != 32 && f_string[i] == 32) res++;
  }
  return res;
}

void scaling_image(Vertex *vertexes, double scaling) {
  for (int i = 0; i < (vertexes->counter_vertex + 1) * 3; i++) {
    vertexes->coordinates[i] = vertexes->coordinates[i] / scaling;
  }
}

void displace(Vertex *vertexes, Step step) {
  int i = 0;
  while (i < (vertexes->counter_vertex + 1) * 3) {
    vertexes->coordinates[i] = vertexes->coordinates[i] + step.dx;
    vertexes->coordinates[i + 1] = vertexes->coordinates[i + 1] + step.dy;
    vertexes->coordinates[i + 2] = vertexes->coordinates[i + 2] + step.dz;
    i = i + 3;
  }
}

void rotation_model(Vertex *vertexes, Rotation rotation) {
  int axis_1 = 0;
  int axis_2 = 0;
  if (rotation.rx != 0) {
    axis_1 = 1;
    axis_2 = 2;
    double radian = grad_to_rad(rotation.rx);
    values_after_rotation(vertexes, radian, axis_1, axis_2);
  }
  if (rotation.ry != 0) {
    axis_1 = 0;
    axis_2 = 2;
    double radian = grad_to_rad(rotation.ry);
    values_after_rotation(vertexes, radian, axis_1, axis_2);
  }
  if (rotation.rz != 0) {
    axis_1 = 0;
    axis_2 = 1;
    double radian = grad_to_rad(rotation.rz);
    values_after_rotation(vertexes, radian, axis_1, axis_2);
  }
}

double grad_to_rad(double grad) {
  double radian = (grad * PI) / 180;
  return radian;
}

void values_after_rotation(Vertex *vertexes, double angle_rotation, int axis_1,
                           int axis_2) {
  int counter = 3;
  int stop = (vertexes->counter_vertex + 1) * 3;
  while (counter < stop) {
    double variable_1 = vertexes->coordinates[counter + axis_1];
    double variable_2 = vertexes->coordinates[counter + axis_2];
    vertexes->coordinates[counter + axis_1] =
        cos(angle_rotation) * variable_1 - sin(angle_rotation) * variable_2;
    vertexes->coordinates[counter + axis_2] =
        sin(angle_rotation) * variable_1 + cos(angle_rotation) * variable_2;
    counter = counter + 3;
  }
}
