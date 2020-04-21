
#include <vtkInteractorStyleRubberBand3D.h>

class MyRubberBand3D : public vtkInteractorStyleRubberBand3D
{
  public:
    static MyRubberBand3D* New();
    vtkTypeMacro(MyRubberBand3D, vtkInteractorStyleRubberBand3D);

    virtual void OnLeftButtonUp() 
    {
      // Forward events
      vtkInteractorStyleRubberBand3D::OnLeftButtonUp();
 
      std::cout << "Start position: " << this->StartPosition[0] << " " << this->StartPosition[1] << std::endl;
      std::cout << "End position: " << this->EndPosition[0] << " " << this->EndPosition[1] << std::endl;
    }
 
};