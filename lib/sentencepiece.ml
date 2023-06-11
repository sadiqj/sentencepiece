type t

external load : string -> t = "caml_sentencepiece_load"
external encode : t -> string -> int list = "caml_sentencepiece_encode"
external decode : t -> int list -> string = "caml_sentencepiece_decode"