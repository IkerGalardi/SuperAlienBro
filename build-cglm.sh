cd ext/cglm/

cmake . -DCGLM_SHARED=OFF -DCGLM_STATIC=ON
make $nproc
mv libcglm.a ../../bin/