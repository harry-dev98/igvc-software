# Multiclass Semantic Segmentation System

Using transfer learning, we can simultaneously classify lines and barrels in images. A U-Net with a pretrained EfficientNet encoder performs multiclass semantic segmentation, classifying the pixels in each image as a line, barrel, or neither. This system uses the [Catalyst](https://github.com/catalyst-team/catalyst) and [Segmentation Models PyTorch](https://github.com/qubvel/segmentation_models.pytorch) libraries. 

<p align="center">
  <img src="https://github.com/suhacker1/igvc-software/blob/multiclass_segmentation/igvc_perception/src/multiclass_segmentation/model_prediction.png">
</p>

## Folder Structure 
+ **UNetWithEfficientNet.ipynb**: implements the training and testing of the neural network in a notebook
+ **UNetWithEfficientNet.pdf**: provides a PDF version of the aforementioned notebook for readability
+ **data_loaders.py**: defines the Torch data loaders
+ **helper_operations.py**: defines multiple operations used for loss and metric calculations
+ **make_dataset.py**: converts folders of images and masks into .npy files
+ **requirements.txt**: lists dependencies
+ **segmentation_dataset.py**: defines the utilized dataset
+ **split_data.py**: splits images and masks into training and testing sets
+ **train.py**: implements, trains, and tests the model

## Build Instructions 

1. Download the dependencies in requirements.txt manually or using `pip install -r requirements.txt`. 
2. Obtain a dataset containing the images (PNGs) and masks (JSONs). Skip steps #2 and #3 if using .npy files. 
3. Run `python make_dataset.py -images /path_to_images(png) -masks /path_to_masks(json)`to generate .npy files. Example: `python make_dataset.py -images 'annotations/**/*.png' -masks 'annotations/**/*.json'`
4. Establish train_images.npy, train_masks.npy, test_images.npy, and test_masks.npy files with `python split_data.py -images '/path_to_images.npy' -masks '/path_to_masks.npy'` 
5. Run `python train.py -train_images '/path_to_train_images' -train_masks '/path_to_train_masks' -test_images '/path_to_test_images' -test_masks '/path_to_test_masks'` to train and test the neural network.

Alternatively, download **UNetWithEfficientNet.ipynb**, and run it in [Google Colaboratory](https://colab.research.google.com/notebooks/intro.ipynb#recent=true) or a local Jupyter environment. 
