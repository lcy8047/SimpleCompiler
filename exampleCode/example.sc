func add(x, y) { return x+y; }

func sub(x, y) { return x-y; }

func mul(x, y) { return x*y; }

func div(x, y) { return x/y; }

func main() {
    var a;
    var b;
    var c;

    a = 6;
    b = 4;

    c = add(a, b);
    c = sub(a, b);
    c = mul(a, b);
    c = div(a, b);

    return;
}