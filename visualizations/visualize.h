#include <string>

#include <ftpip/ftpip.h>
#include <ftpip/find_borders.h>
#include <twg/image.h>
#include <twg/image/image_drawing.h>

/** Рисует картинку вокруг полигона, где красным отображается неправильный ответ на вопрос о принадлежности точки полигону при помощи дерева, а зеленый - правильный ответ. Используется для проверки того, правильно ли работает дерево. */
void check_work(const std::vector<spob::vec2>& polygon, ftpip::TreeElem_ptr tree,
                const std::wstring& name, int size, int border);

/** Рисует текущую часть и текущее разделение */
void drawCurrentTree(twg::ImageDrawing_aa& img, 
                     const spob::FindBorders& brd,
                     ftpip::TreeElem_ptr tree, 
                     ftpip::TreeElem_ptr current,
                     const std::vector<spob::vec2>& polygon, 
                     const glm::mat3& currentTransformation);

/** Представляет текущее дерево в графическом виде, для дальнейшего изучения. */
void drawAllTree(const std::wstring& filename, 
				 int size, int insideBorder, int padding, 
                 ftpip::TreeElem_ptr tree, 
                 const std::vector<spob::vec2>& polygon);