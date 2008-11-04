#!/bin/sh

rm -rf ../echo_model*
./gen.sh body  
./gen.sh foot  
./gen.sh head  
./gen.sh left_hand  
./gen.sh limb  
./gen.sh lower_body  
./gen.sh right_hand  
./gen.sh sphere 0.075f
mv ../echo_model_sphere.cpp ../echo_model_sphere_pointzero75.cpp
sed -i 's|draw_sphere|draw_sphere_pointzero75|' ../echo_model_sphere_pointzero75.cpp
./gen.sh sphere 0.1f
mv ../echo_model_sphere.cpp ../echo_model_sphere_point1.cpp
sed -i 's|draw_sphere|draw_sphere_point1|' ../echo_model_sphere_point1.cpp
./gen.sh stairs 
