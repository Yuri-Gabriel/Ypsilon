function recursive(number num): void {
    if(num == 10) {
        return;
    }
    print(num);
    num += 1;
    return recursive(num);
}

function square(number num): number {
    number two = 2;
    return num ^ two;
}

