use std::collections::HashMap;
use std::fs;

fn dfs<'a>(
    front: &'a str,
    goal: &'a str,
    devices: &'a HashMap<String, Vec<String>>,
    memo: &mut HashMap<&'a str, i64>,
) -> i64 {
    if let Some(&v) = memo.get(front) {
        return v;
    }
    if front == goal {
        return 1;
    } else if front == "out" {
        return 0;
    }
    let res = devices[front]
        .iter()
        .map(|n| dfs(n, goal, devices, memo))
        .sum();
    memo.insert(front, res);
    res
}

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut devices = HashMap::new();
    for line in file.lines() {
        let mut ite = line.split(':');
        let first = ite.next().unwrap();
        let connected_to = devices.entry(first.to_string()).or_insert(Vec::new());
        let rest = ite.next().unwrap().trim();
        for d in rest.split(' ') {
            connected_to.push(d.to_string());
        }
    }
    let mut memo: HashMap<&str, i64> = HashMap::new();
    let res_1 = dfs("svr", "out", &devices, &mut memo);

    //Part 2
    let path = vec!["svr", "fft", "dac", "out"];
    let mut res_2 = 1;
    for i in 0..(path.len() - 1) {
        memo.clear();
        let temp = dfs(path[i], path[i + 1], &devices, &mut memo) as i128;
        res_2 *= temp;
    }
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
}
