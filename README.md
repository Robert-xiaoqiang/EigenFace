# PCA method for face recognition

## build prerequisite
   - C++17 (`std::filesystem`) support
   - OpenCV 3.X library

## experimental case

   - `> TrainMain #faces modelName facesDataSet`

      - `> TrainMain 50 model/faces.model train`


   - `> TestMain <facesDirName to recognition> modelName facesDataSet`

      - `> TestMain test model/faces.model train`

## directory
   - `/haar/` eye detector for face alignment
   - `/model/` engien faces
   - `/partial_results/` results of face recognition

## the avarage face and the first 10 eigenfaces
![tenth_eigenface](tenth_eigenface.png '1')

## some results
- (source picture, build using eigenfaces, plus source picture, similar in dataset)
  ![2](partial_results/me.0.png_result.png '2')
  <br/>
  ![3](partial_results/YM.AN1.61.tiff_result.png '3')



## note
   - copy JAFFE to `train directory` before training
   - picture naming conventions `name.*.*.tiff/png`

