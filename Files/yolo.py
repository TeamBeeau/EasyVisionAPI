import torch
import cv2
from ultralytics import YOLO
import time
import numpy as np
#from MatCV import process_image  # Giả sử bạn đã tạo module này
# Đường dẫn đến mô hình YOLOv8 (.pt) và ảnh bạn muốn kiểm tra

model= None
class ObjectDetector:
    def __init__(self):
        self.model = None
    @staticmethod
    def load_model(model_path):
            global model 
                # Tải mô hình YOLOv8 từ tệp .pt
            model = YOLO(model_path)
           
            return model
    @staticmethod
    def predict(image_array, confidence_threshold):
            #image_array = cv2.imread(image_path)
            if isinstance(image_array, np.ndarray):
               
                #input_tensor = torch.from_numpy(raw).permute(2, 0, 1).float() / 255.0
                    # Thêm một chiều batch
                #input_tensor = input_tensor.unsqueeze(0)
                global model  # Khai báo rằng chúng ta đang sử dụng biến toàn cục
                if model is None:
                    raise ValueError("Mô hình chưa được tải. Vui lòng tải mô hình trước khi dự đoán.")
                    
                results = model.predict(image_array, save=False, show=False, conf=confidence_threshold)
                if (len(results) == 0):
                    raise ValueError("no box 11")
                if isinstance(results, list) and len(results) > 0:
                    for result in results:
                        #print(result)
                        #print(results[0].boxes)
                        
                        if hasattr(result, 'boxes'):  # Kiểm tra xem đối tượng có thuộc tính 'boxes' không
                            boxes = result.boxes  # Lấy các bounding boxes
                            valid_boxes = []
                            scores = []

                            # Nếu boxes là tensor
                            for box in boxes.data:
                                x1, y1, x2, y2, conf, cls = box.tolist()  # Chuyển đổi tensor thành danh sách
                                class_id = int(cls)  # Đảm bảo ID lớp là số nguyên
                                #if(model.names[class_id]!="dauloc"):
                                 #      continue
                                valid_boxes.append((int(x1), int(y1), int(x2), int(y2)))
                                scores.append(conf)
                            
                            print("Bounding Boxes:", valid_boxes)
                            print("Scores:", scores)
                            return valid_boxes, scores
                        else:
                            print("Đối tượng không có thuộc tính 'boxes'")
                    
                else:
                    print("Không có bounding boxes được phát hiện.")
                    raise ValueError("no box")      
            else:
                raise ValueError("Input must be a numpy array")
#load_model(model_path)
#predict(0.5)
