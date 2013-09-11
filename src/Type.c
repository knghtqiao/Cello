#include "Cello/Type.h"

#include "Cello/Bool.h"
#include "Cello/Exception.h"
#include "Cello/Number.h"
#include "Cello/String.h"

#include <string.h>

var Type = type_data {
  type_begin(Type),
  type_entry(Type, New),
  type_entry(Type, AsStr),
  type_entry(Type, Show),
  type_end(Type),
};

var Type_Cast(var obj, var t, const char* func, const char* file, int line) {
  
  if (type_of(obj) is t) {
    return obj;
  } else {
    return throw(TypeError,
      "Argument to function '%s' at '%s:%i' :: "
      "Got Type '%s' :: "
      "Expected Type '%s'", 
      $(String, (char*)func), $(String, (char*)file), $(Int, line), type_of(obj), t);
  }
  
}

var Type_New(var self, var_list vl) {
  
  const char* name = as_str(var_list_get(vl));
  int count = as_long(var_list_get(vl));
  var* ifaces = var_list_get(vl);
  const char** inames = var_list_get(vl);
  
  TypeData* newtype = malloc(sizeof(TypeData) * (count + 2));
  
  newtype[0].class_object = NULL;
  newtype[0].class_name = "__Type";
  
  newtype[1].class_object = (void*)name;
  newtype[1].class_name = "__Name";
  
  for(int i = 0; i < count; i++) {
    newtype[2+i].class_object = ifaces[i];
    newtype[2+i].class_name = inames[i];
  }
  
  return newtype;
}

var Type_Delete(var self) {
  return self;
}

size_t Type_Size(void) {
  return 0;
}

const char* Type_AsStr(var self) {
  return type_class(self, __Name);
}

var Type_Implements_Name(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) return True;
    t++;
  }
  
  return False;
}

var Type_Implements_Method_Name(var self, int offset, const char* class_name, const char* func, const char* file, int line) {
  
  if (not Type_Implements_Name(self, class_name, func, file, line)) {
    return False;
  } else {
    intptr_t* func_address = Type_Class_Name(self, class_name, func, file, line) + offset;
    return bool_var(*func_address);
  }
  
}

var Type_Class_Name(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  const char* type_name = NULL;
  
  while(t->class_name) {
    if (strcmp(t->class_name, "__Name") == 0) {
      type_name = t->class_object;
    }
    if (strcmp(t->class_name, class_name) == 0) {
      return (var)t->class_object;
    }
    t++;
  }
  
  if (type_name == NULL) {
  
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Cannot find class '__Name' for object '%p' :: "
      "Was is correctly Constructed? :: "
      "Does it start with a 'type' entry? ::"
      "Was `methods_begin` used?",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
      
  } else {
    
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Type '%s' does not implement class '%s'",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), 
      $(String, (char*)type_name), $(String, (char*)class_name));
      
  }
}

var Type_Class_Name_Method(var self, var method, const char* class_name, const char* method_name, const char* func, const char* file, int line) {
  
  var c = Type_Class_Name(self, class_name, func, file, line);
  
  if (method == NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Type '%s' implements class '%s' but not the specific method '%s' required",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), 
      self, $(String, (char*)class_name), $(String, (char*)method_name));
  } else {
    return c;
  }
  
}

int Type_Show(var self, var output, int pos) {
  return print_to(output, pos, "%s", self);
}


