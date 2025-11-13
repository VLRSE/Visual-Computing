set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "C:/Users/marti/OneDrive/Dokumente/Vilrose_HDA/Visual Computing/7WiSe25_26/Praktikum/Prak1/VC_OpenGL_Praktikum_Framework/OpenGL_Praktikum/libs/glm-master")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
