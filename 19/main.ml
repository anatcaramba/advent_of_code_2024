let read_patterns ic =
    let liste_val = String.split_on_char ',' (String.trim (input_line ic)) in
    let rec aux li =
        match li with [] -> [] | s :: li' -> String.trim s :: aux li'
    in
    aux liste_val

let read_designs ic =
    let lines = ref [] in
    try
      while true do
        lines := String.trim (input_line ic) :: !lines
      done;
      !lines
    with End_of_file ->
      close_in ic;
      List.rev !lines

let suivant (patt : string) (s : string) (i : int) =
    let n = String.length s in
    if i = n then Some n
    else
      let m = String.length patt in
      let poss = ref true in
      for j = 0 to m - 1 do
        if i + j >= n || patt.[j] != s.[i + j] then poss := false
      done;
      if !poss then Some (i + m) else None

let tous_suivants patterns s i =
    List.filter_map (fun x -> suivant x s i) patterns

let est_possible patterns s =
    let n = String.length s in
    let rec aux positions =
        if positions = [] then false
        else if List.exists (fun x -> x = n) positions then true
        else
          let positions' =
              List.sort_uniq compare
                (List.concat
                   (List.map (fun x -> tous_suivants patterns s x) positions))
          in
          aux positions'
    in
    aux [ 0 ]

let coincide_i p s i = match suivant p s i with None -> false | Some _ -> true

let coincide p s =
    List.exists
      (fun i -> coincide_i p s i)
      (List.init (String.length s) (fun i -> i))

let rec patt_presents patterns s = List.filter (fun p -> coincide p s) patterns

let rec combien_acc_simple patterns designs =
    match designs with
    | [] -> 0
    | s :: designs' ->
        let pp = patt_presents patterns s in
        (if est_possible pp s then 1 else 0)
        + combien_acc_simple patterns designs'

let nb_possibles patterns s =
    let n = String.length s in
    let nb = Array.make (n + 1) 0 in
    nb.(0) <- 1;
    let rec remplir patterns i =
        match patterns with
        | [] -> ()
        | patt :: patterns' ->
            if coincide_i patt s i then
              nb.(i + String.length patt) <-
                nb.(i + String.length patt) + nb.(i);
            remplir patterns' i
    in
    for i = 0 to n - 1 do
      remplir patterns i
    done;
    nb.(n)

let rec combien_acc_mult patterns designs =
    match designs with
    | [] -> 0
    | s :: designs' ->
        let pp = patterns in
        let x = nb_possibles pp s in
        x + combien_acc_mult patterns designs'

let main =
    let f = open_in "./input" in
    let patterns = read_patterns f in
    let _ = input_line f in
    let designs = read_designs f in
    let ret1 = combien_acc_simple patterns designs in
    print_int ret1;
    print_newline ();
    let ret2 = combien_acc_mult patterns designs in
    print_int ret2;
    print_newline ()
