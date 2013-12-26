/*
** userdata.c - userdata module
**
** See Copyright Notice in mruby.h
*/

#include <stdarg.h>
#include <string.h>

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/data.h"
#include "mruby/hash.h"
#include "mruby/string.h"

#define MRB_USERDATA_KEY_STORE    "mrb_userdata_key_store"
#define MRB_USERDATA_DEFAULT_KEY  "mrb_userdata_default_key"

#define DONE mrb_gc_arena_restore(mrb, 0);
#if 1
#define ARENA_SAVE \
  int ai = mrb_gc_arena_save(mrb); \
  if (ai == MRB_ARENA_SIZE) { \
    mrb_raise(mrb, E_RUNTIME_ERROR, "arena overflow"); \
  }
#define ARENA_RESTORE \
  mrb_gc_arena_restore(mrb, ai);
#else
#define ARENA_SAVE
#define ARENA_RESTORE
#endif

/*
** Userdata#
*/

static mrb_value mrb_userdata_init(mrb_state *mrb, mrb_value self)
{
  mrb_value userdata, userdata_key;

  if (mrb_get_args(mrb, "|S", &userdata_key) == 0) {
    userdata_key = mrb_str_new_cstr(mrb, MRB_USERDATA_DEFAULT_KEY);
  }
  userdata = mrb_gv_get(mrb, mrb_intern_cstr(mrb, mrb_str_to_cstr(mrb, userdata_key)));
  if (mrb_nil_p(userdata)) {
    userdata = mrb_hash_new(mrb);  
  }
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, MRB_USERDATA_KEY_STORE), userdata_key);
  mrb_gv_set(mrb, mrb_intern_cstr(mrb, mrb_str_to_cstr(mrb, userdata_key)), userdata);

  return userdata;
}

static mrb_value mrb_userdata_get(mrb_state *mrb, mrb_value self, mrb_value key)
{
  mrb_value userdata_key = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, MRB_USERDATA_KEY_STORE));
  mrb_value userdata = mrb_gv_get(mrb, mrb_intern_cstr(mrb, mrb_str_to_cstr(mrb, userdata_key)));

  if (mrb_nil_p(userdata)) {
    userdata = mrb_hash_new(mrb);
  }
  return mrb_hash_get(mrb, userdata, key);
}

static mrb_value mrb_userdata_set(mrb_state *mrb, mrb_value self, mrb_value key, mrb_value val)
{
  mrb_value userdata_key = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, MRB_USERDATA_KEY_STORE));
  mrb_value userdata = mrb_gv_get(mrb, mrb_intern_cstr(mrb, mrb_str_to_cstr(mrb, userdata_key)));

  if (mrb_nil_p(userdata)) {
    userdata = mrb_hash_new(mrb);  
  }
  mrb_hash_set(mrb, userdata, key, val);
  mrb_gv_set(mrb, mrb_intern_cstr(mrb, mrb_str_to_cstr(mrb, userdata_key)), userdata);

  return key;
}

static mrb_value mrb_userdata_method_missing(mrb_state *mrb, mrb_value self)
{
  mrb_value name, *a;
  mrb_value s_name;
  char *c_name;
  int alen;
  size_t len;

  mrb_get_args(mrb, "n*", &name, &a, &alen);

  s_name = mrb_sym2str(mrb, mrb_symbol(name));
  c_name = mrb_str_to_cstr(mrb, s_name);
  len = strlen(c_name); 

  if (c_name[len-1] == '=') {
    return mrb_userdata_set(mrb, self, mrb_str_new(mrb, c_name, len-1), a[0]);
  } else {
    return mrb_userdata_get(mrb, self, s_name);
  }
}

void mrb_mruby_userdata_gem_init(mrb_state *mrb)
{
  struct RClass *userdata;

  userdata = mrb_define_class(mrb, "Userdata", mrb->object_class);
  mrb_define_method(mrb, userdata, "initialize", mrb_userdata_init, ARGS_NONE());
  mrb_define_method(mrb, userdata, "method_missing", mrb_userdata_method_missing, ARGS_ANY());
  DONE;

}

void mrb_mruby_userdata_gem_final(mrb_state *mrb)
{
}

