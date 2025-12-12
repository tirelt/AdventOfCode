use std::fs;

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let n_shapes = 6;
    let mut counter = 0;
    let mut shapes = Vec::new();
    let mut sizes = Vec::new();
    let mut temp = Vec::new();
    let mut size = 0;
    for line in file.lines() {
        if counter % 5 == 0 {
            counter += 1;
            continue;
        }
        if counter % 5 == 4 {
            shapes.push(temp.clone());
            sizes.push(size);
            temp.clear();
            size = 0;
            counter += 1;
            if shapes.len() == n_shapes {
                break;
            }
            continue;
        }
        let mut v = Vec::new();

        for c in line.chars() {
            if c == '#' {
                size += 1;
            }
            v.push(c);
        }
        temp.push(v);
        counter += 1;
    }
    counter = 0;
    let mut res_1 = 0;
    for line in file.lines() {
        if counter > 29 {
            let l: i32 = line[0..=1].parse().unwrap();
            let w: i32 = line[3..=4].parse().unwrap();
            let mut i = 0;
            let mut min = 0;
            let mut max = 0;
            for num_str in line[7..].split(' ') {
                let num: i32 = num_str.parse().unwrap();
                min += sizes[i] * num;
                max += 9 * num;
                i += 1;
            }
            let capa = l * w;
            if capa > max {
                res_1 += 1;
            }
            if capa < max && capa > min {
                println!("capa: {capa} - min: {min} - max: {max}");
                println!("tricky");
            }
        }
        counter += 1;
    }
    println!("Part 1: {res_1}");
}
