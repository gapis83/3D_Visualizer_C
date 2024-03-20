#include <check.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_viewer.c"
#include "s21_viewer.h"

START_TEST(emty_file_test) {
  Vertex test_1v = {0, NULL};
  Facet test_1f = {0, 0, NULL};
  Vertex test_11v = {0, NULL};
  Facet test_11f = {0, 0, NULL};
  char *f_name1 = "./models/Empty.obj";
  char *f_name2 = "./models/ty.obj";
  char *f_name3 = NULL;
  int error = 0;
  int error2 = 0;
  error = parser(f_name1, &test_1v, &test_1f);
  error2 = parser(f_name3, &test_11v, &test_11f);

  ck_assert_int_eq(error, 4);
  ck_assert_int_eq(error2, 1);
  ck_assert_int_eq(test_11v.counter_vertex, -1);

  free(test_1v.coordinates);
  test_1v.counter_vertex = 0;
  free(test_1f.lines);
  test_1f.counter_facets = 0;
  test_1f.counter_lines = 0;

  error = parser(f_name2, &test_1v, &test_1f);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(correct_value_test) {
  Vertex test_2v = {0, NULL};
  Facet test_2f = {0, 0, NULL};
  char *f_name = "./models/cube.obj";
  parser(f_name, &test_2v, &test_2f);
  double cube_vertex[] = {0.0,  0.0,  0.0, 1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,
                          -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0,
                          1.0,  1.0,  1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0};
  unsigned cube_facet[] = {
      2, 3, 3, 4, 4, 2, 8, 7, 7, 6, 6, 8, 5, 6, 6, 2, 2, 5, 6, 7, 7, 3, 3, 6,
      3, 7, 7, 8, 8, 3, 1, 4, 4, 8, 8, 1, 1, 2, 2, 4, 4, 1, 5, 8, 8, 6, 6, 5,
      1, 5, 5, 2, 2, 1, 2, 6, 6, 3, 3, 2, 4, 3, 3, 8, 8, 4, 5, 1, 1, 8, 8, 5};
  for (int i = 0; i < 27; i++) {
    ck_assert_double_eq(test_2v.coordinates[i], cube_vertex[i]);
  }
  for (int i = 0; i < 36; i++) {
    ck_assert_int_eq(test_2f.lines[i], cube_facet[i]);
  }

  free(test_2v.coordinates);
  test_2v.counter_vertex = 0;
  free(test_2f.lines);
  test_2f.counter_facets = 0;
  test_2f.counter_lines = 0;
}
END_TEST

START_TEST(zoom_test) {
  Vertex test_3v = {0, NULL};
  Facet test_3f = {0, 0, NULL};
  Vertex zoom_vertex = {0, NULL};
  Facet zoom_facet = {0, 0, NULL};
  double zoom = 1.5;
  char *f_name = "./models/cube.obj";
  parser(f_name, &test_3v, &test_3f);
  parser(f_name, &zoom_vertex, &zoom_facet);
  scaling_image(&test_3v, zoom);

  for (int i = 3; i < test_3v.counter_vertex; i++) {
    ck_assert_double_eq(test_3v.coordinates[i],
                        zoom_vertex.coordinates[i] / zoom);
  }
  ck_assert_int_eq(test_3v.counter_vertex, 8);
  ck_assert_int_eq(test_3f.counter_facets, 12);
  ck_assert_int_eq(test_3f.counter_lines, 37);

  test_3v.counter_vertex = 0;
  free(test_3v.coordinates);
  free(test_3f.lines);
  test_3f.counter_facets = 0;
  test_3f.counter_lines = 0;
  zoom_vertex.counter_vertex = 0;
  free(zoom_vertex.coordinates);
  free(zoom_facet.lines);
}
END_TEST

START_TEST(displace_test) {
  Vertex test_4v = {0, NULL};
  Facet test_4f = {0, 0, NULL};
  Vertex step_vertex = {0, NULL};
  Facet step_facet = {0, 0, NULL};
  Step test_step = {0, 0, 0};
  test_step.dx = 1.1;
  test_step.dy = -1.2;
  test_step.dz = 0.03;
  char *f_name = "./models/cube.obj";

  parser(f_name, &test_4v, &test_4f);
  parser(f_name, &step_vertex, &step_facet);
  displace(&step_vertex, test_step);

  for (int i = 0; i < test_4v.counter_vertex; i += 3) {
    ck_assert_double_eq_tol(test_4v.coordinates[i],
                            step_vertex.coordinates[i] - test_step.dx, 1e-6);
    ck_assert_double_eq_tol(test_4v.coordinates[i + 1],
                            step_vertex.coordinates[i + 1] - test_step.dy,
                            1e-6);
    ck_assert_double_eq_tol(test_4v.coordinates[i + 2],
                            step_vertex.coordinates[i + 2] - test_step.dz,
                            1e-6);
  }

  free(test_4v.coordinates);
  test_4v.counter_vertex = 0;
  free(test_4f.lines);
  test_4f.counter_facets = 0;
  test_4f.counter_lines = 0;
  free(step_vertex.coordinates);
  free(step_facet.lines);
  step_facet.counter_facets = 0;
  step_facet.counter_lines = 0;
}
END_TEST

START_TEST(rotation_test_x) {
  Vertex test_5v = {0, NULL};
  Facet test_5f = {0, 0, NULL};
  Vertex turn_vertex = {0, NULL};
  Facet turn_f1 = {0, 0, NULL};
  Rotation test_rotation = {-45, 0, 0};
  char *f_name = "./models/cube.obj";

  parser(f_name, &test_5v, &test_5f);
  parser(f_name, &turn_vertex, &turn_f1);
  rotation_model(&test_5v, test_rotation);
  double radian = grad_to_rad(test_rotation.rx);
  for (int i = 3; i < (test_5v.counter_vertex + 1) * 3; i += 3) {
    double variable_1 = cos(radian) * turn_vertex.coordinates[i + 1] -
                        (sin(radian) * turn_vertex.coordinates[i + 2]);
    double variable_2 = sin(radian) * turn_vertex.coordinates[i + 1] +
                        cos(radian) * turn_vertex.coordinates[i + 2];
    ck_assert_double_eq_tol(test_5v.coordinates[i], turn_vertex.coordinates[i],
                            1e-6);
    ck_assert_double_eq_tol(test_5v.coordinates[i + 1], variable_1, 1e-6);
    ck_assert_double_eq_tol(test_5v.coordinates[i + 2], variable_2, 1e-6);
  }

  free(test_5v.coordinates);
  test_5v.counter_vertex = 0;
  free(test_5f.lines);
  test_5f.counter_facets = 0;
  test_5f.counter_lines = 0;
  free(turn_vertex.coordinates);
  turn_vertex.counter_vertex = 0;
  free(turn_f1.lines);
  turn_f1.counter_facets = 0;
  turn_f1.counter_lines = 0;
}
END_TEST

START_TEST(rotation_test_y) {
  Vertex test_5v = {0, NULL};
  Facet test_5f = {0, 0, NULL};
  Vertex turn_vertex = {0, NULL};
  Facet turn_f2 = {0, 0, NULL};
  Rotation test_rotation = {0, -90.5, 0};
  char *f_name = "./models/cube.obj";

  parser(f_name, &test_5v, &test_5f);
  parser(f_name, &turn_vertex, &turn_f2);
  rotation_model(&test_5v, test_rotation);
  double radian = grad_to_rad(test_rotation.ry);
  for (int i = 3; i < (test_5v.counter_vertex + 1) * 3; i += 3) {
    double variable_1 = cos(radian) * turn_vertex.coordinates[i] -
                        (sin(radian) * turn_vertex.coordinates[i + 2]);
    double variable_2 = sin(radian) * turn_vertex.coordinates[i] +
                        cos(radian) * turn_vertex.coordinates[i + 2];

    ck_assert_double_eq_tol(test_5v.coordinates[i], variable_1, 1e-6);
    ck_assert_double_eq_tol(test_5v.coordinates[i + 2], variable_2, 1e-6);
  }

  free(test_5v.coordinates);
  test_5v.counter_vertex = 0;
  free(test_5f.lines);
  test_5f.counter_facets = 0;
  test_5f.counter_lines = 0;
  free(turn_vertex.coordinates);
  turn_vertex.counter_vertex = 0;
  free(turn_f2.lines);
  turn_f2.counter_facets = 0;
  turn_f2.counter_lines = 0;
}
END_TEST

START_TEST(rotation_test_z) {
  Vertex test_5v = {0, NULL};
  Facet test_5f = {0, 0, NULL};
  Vertex turn_vertex = {0, NULL};
  Facet turn_f3 = {0, 0, NULL};
  Rotation test_rotation = {0, 0, 0};
  test_rotation.rz = 30;
  char *f_name = "./models/cube.obj";

  parser(f_name, &test_5v, &test_5f);
  parser(f_name, &turn_vertex, &turn_f3);
  rotation_model(&test_5v, test_rotation);
  double radian = grad_to_rad(test_rotation.rz);
  for (int i = 3; i < (test_5v.counter_vertex + 1) * 3; i += 3) {
    double variable_1 = (cos(radian) * turn_vertex.coordinates[i]) -
                        (sin(radian) * turn_vertex.coordinates[i + 1]);
    double variable_2 = sin(radian) * turn_vertex.coordinates[i] +
                        cos(radian) * turn_vertex.coordinates[i + 1];
    ck_assert_double_eq_tol(test_5v.coordinates[i], variable_1, 1e-6);
    ck_assert_double_eq_tol(test_5v.coordinates[i + 1], variable_2, 1e-6);
    ck_assert_double_eq_tol(test_5v.coordinates[i + 2],
                            turn_vertex.coordinates[i + 2], 1e-6);
  }

  free(test_5v.coordinates);
  test_5v.counter_vertex = 0;
  free(test_5f.lines);
  test_5f.counter_facets = 0;
  test_5f.counter_lines = 0;
  free(turn_vertex.coordinates);
  turn_vertex.counter_vertex = 0;
  free(turn_f3.lines);
  turn_f3.counter_facets = 0;
  turn_f3.counter_lines = 0;
}
END_TEST

Suite *viewer_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("3D_Viewer_v1.0 test.\n");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, emty_file_test);
  tcase_add_test(tc_core, correct_value_test);
  tcase_add_test(tc_core, zoom_test);
  tcase_add_test(tc_core, displace_test);
  tcase_add_test(tc_core, rotation_test_x);
  tcase_add_test(tc_core, rotation_test_y);
  tcase_add_test(tc_core, rotation_test_z);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = viewer_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
