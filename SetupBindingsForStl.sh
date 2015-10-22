cd ..

rm ./include/bindings/SharedPointer.h
echo "// AUTOMATICALLY MOVED! This file is shadowed!" > ./include/bindings/SharedPointer.h
cat ./bindings/SharedPointerStl.h >> ./include/bindings/SharedPointer.h

rm ./include/bindings/Array.h
echo "// AUTOMATICALLY MOVED! This file is shadowed!" > ./include/bindings/Array.h
cat ./bindings/ArrayStl.h >> ./include/bindings/Array.h

