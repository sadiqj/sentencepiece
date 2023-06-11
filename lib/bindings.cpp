#include "sentencepiece_processor.h"
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <vector>

#define Sentencepiece_val(v) (*((sentencepiece::SentencePieceProcessor **) Data_custom_val(v)))

void sentencepiece_finalize(value processor) {
  sentencepiece::SentencePieceProcessor *p = (sentencepiece::SentencePieceProcessor *) processor;
  delete p;
}

CAMLprim value caml_sentencepiece_load(value path) {
  CAMLparam1(path);
  CAMLlocal1(result);

  struct custom_operations sp_ops = {
    "sentencepiece_processor",
    sentencepiece_finalize,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default
  };

  sentencepiece::SentencePieceProcessor processor;
  const auto status = processor.Load(String_val(path));
  if (!status.ok()) {
    caml_failwith(status.ToString().c_str());
  } else {
    result = caml_alloc_custom(&sp_ops, sizeof(sentencepiece::SentencePieceProcessor *), 0, 1);
    Sentencepiece_val(result) = &processor;
  }

  CAMLreturn(result);
}

CAMLprim value caml_sentencepiece_encode(value processor, value text) {
  CAMLparam2(processor, text);
  CAMLlocal1(result);

  std::vector<std::string> pieces;
  const auto status = Sentencepiece_val(processor)->Encode(String_val(text), &pieces);
  if (!status.ok()) {
    caml_failwith(status.ToString().c_str());
  } else {
    result = caml_alloc(pieces.size(), 0);
    for (size_t i = 0; i < pieces.size(); i++) {
      Store_field(result, i, caml_copy_string(pieces[i].c_str()));
    }
  }

  CAMLreturn(result);
}

CAMLprim value caml_sentencepiece_decode(value processor, value ids) {
  CAMLparam2(processor, ids);
  CAMLlocal1(result);
  /* copy from the ids array in to a fresh vector<int> */
  std::vector<int> ids_vec;
  ids_vec.reserve(Wosize_val(ids));
  for (int i = 0; i < Wosize_val(ids); i++) {
    ids_vec.push_back(Int_val(Field(ids, i)));
  }
  std::string text;
  const auto status = Sentencepiece_val(processor)->Decode(ids_vec, &text);
  if (!status.ok()) {
    caml_failwith(status.ToString().c_str());
  } else {
    result = caml_copy_string(text.c_str());
  }

  CAMLreturn(result);
}