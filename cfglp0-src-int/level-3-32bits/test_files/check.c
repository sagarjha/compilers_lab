int a;

int g() {
  main();
  return 1;
}

int f(int b) {
  return g();
}

main () {
  a = 3;
  int b;
  g();
  f(b);
  return 2.09;
}
