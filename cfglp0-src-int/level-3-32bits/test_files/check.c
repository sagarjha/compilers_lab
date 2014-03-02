int a;

int g() {
  return a;
}

int f() {
  return g();
}

main () {
  a = 3;
  return f();
}
