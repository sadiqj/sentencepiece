type t

val load : string -> t
val encode : t -> string -> int list
val decode : t -> int list -> string