import cv2


def invert_font_color(path):
    image = cv2.imread(path, cv2.IMREAD_UNCHANGED)

    b, g, r, a = cv2.split(image)

    contours, hierarchy = cv2.findContours(a, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

    for contour in contours:
        xOffset, yOffset, width, height = cv2.boundingRect(contour)
        
        roi=image[yOffset:,xOffset:][:height,:width]        
        mask_roi=a[yOffset:,xOffset:][:height,:width]

        mean_scalar = cv2.mean(roi,mask_roi)
        if mean_scalar[0] > 230:
            image[yOffset:,xOffset:][:height,:width][:, :, 0] = 0
            image[yOffset:,xOffset:][:height,:width][:, :, 1] = 0
            image[yOffset:,xOffset:][:height,:width][:, :, 2] = 0

    cv2.imwrite("out.png", image)


if __name__ == '__main__':
    invert_font_color(".\\frog.png")
