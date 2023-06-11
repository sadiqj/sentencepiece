let sp = Sentencepiece.load("test_model.model") in
  let ids = Sentencepiece.encode sp "Hello world!" in
  let text = Sentencepiece.decode sp ids in
  Printf.printf "%s\n" text