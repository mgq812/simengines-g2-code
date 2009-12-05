Just in case you may be wondering where the import libs are for MinGW build, don't worry. 
MinGW/gcc does not use import libs, link directly to the static libs for static builds, 
or the dlls for dynamic builds.