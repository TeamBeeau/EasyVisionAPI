import torch
import cv2
from ultralytics import YOLO
import time
import numpy as np
pathModel="model.pt"
imagPath="test.png"
model= None
class ObjectDetector:
    def __init__(self):
        self.model = None
    @staticmethod
    def load_model(model_path):
            global model 
            model = YOLO(model_path)
            return model
    @staticmethod
    def predict(image_array, confidence_threshold,nms=0.9):
            #image_array = cv2.imread(image_path)
            #imageRS=image_array
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
                            total_width = 0
                            total_height = 0
                            pixel_wire_count = 0

                            # Nếu boxes là tensor
                            for box in boxes.data:
                                x1, y1, x2, y2, conf, cls = box.tolist()  # Chuyển đổi tensor thành danh sách
                                class_id = int(cls)  # Đảm bảo ID lớp là số nguyên
                                width = x2 - x1 
                                height = y2 - y1  

                                #if width <= 100 and height >= 60:
                                valid_boxes.append((int(x1), 0, int(x2), 98))
                                scores.append(conf)
                                total_width += width
                                pixel_wire_count += 1
                            avg_width = total_width / pixel_wire_count if pixel_wire_count > 0 else 0
                            nms_indices = cv2.dnn.NMSBoxes(
                            bboxes=valid_boxes,
                            scores=scores,
                            score_threshold=0.5,  # Ngưỡng độ tin cậy
                            nms_threshold=nms    # Ngưỡng NMS
                            )
                            boxeRS = []
                            scoreRS = []
                            # Hiển thị các bounding box không bị loại bỏ
                            for i in nms_indices.flatten():
                                scoreRS.append(scores[i])
                                boxeRS.append(valid_boxes[i])
                                x_min, y_min, x_max, y_max = valid_boxes[i]
                                #label = int(classes[i])
                                confidence = scores[i]
                                # Vẽ bounding box
                                #cv2.rectangle(imageRS, (int(x_min), int(y_min)), (int(x_max), int(y_max)), (0, 255, 0), 2)
                                #text = f"Class: {label}, Conf: {confidence:.2f}"
                                #cv2.putText(image, text, (int(x_min), int(y_min - 10)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)
                            # Hiển thị ảnh
                           
                            print("Bounding Boxes:", len(boxeRS))
                            print("Scores:", scores)
                            return boxeRS, boxeRS, avg_width
                        else:
                            print("Đối tượng không có thuộc tính 'boxes'")
                    
                else:
                    print("Không có bounding boxes được phát hiện.")
                    raise ValueError("no box")      
            else:
                raise ValueError("Input must be a numpy array")
    @staticmethod
    def close():
        global model
        if model is not None:
            model = None  # Xóa tham chiếu đến mô hình
            print("Mô hình đã được giải phóng.")
        else:
            print("Không có mô hình để giải phóng.")
#ObjectDetector.load_model(pathModel)
#ObjectDetector.predict(imagPath,0.2,0.8)
