#load "Unix.cma"

let calculate_matrix a b c n = 
  for i = 0 to n-1 do
    for k = 0 to n-1 do
      for j = 0 to n-1 do
        c.(i).(k) <- c.(i).(k) + a.(i).(j) + b.(j).(k)
      done;
    done;
  done;;

let init_matrix n a b =
  for i = 0 to n-1 do
    for j = 0 to n-1 do
      a.(i).(j) <- i * n + j; 
      b.(i).(j) <- j * n + i;
    done;
  done;;
  
  
let calculate n =
  let a = Array.make_matrix n n 0 in 
  let b = Array.make_matrix n n 0 in 
  let c = Array.make_matrix n n 0 in
  let () = init_matrix n a b in
  let start = Unix.gettimeofday() in
  let res = calculate_matrix a b c n in
  let stop = Unix.gettimeofday() in 
  let () = Printf.printf "%f\n" (stop -. start) in
  res;;
  
let rec roop n =
  let() = calculate n in  
  if n < 300 then roop (n+10) in
  roop 2;;
