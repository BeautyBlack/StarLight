#include "pch.h"
#include "ImageDisplayerWidget.h"

#include <QPainter>
#include <QTextCursor>
#include <QPoint>
#include <QMouseEvent>
#include <qtextformat.h>
#include <QTextDocument>
#include <qapplication.h>


#include "FloatInputTextWindow.h"
#include "ThemeDefine.h"
#include "utilities/Messenger/EventArgs/UI/ImageFrame/SelectedImageChangedEventArgs.h"
#include "ViewModels/ImageFrameViewModel/Annotation/AngleAnnotation.h"
#include "ViewModels/ImageFrameViewModel/Annotation/CurveLineAnnotation.h"
#include "ViewModels/ImageFrameViewModel/Annotation/LineAnnotation.h"
#include "ViewModels/ImageFrameViewModel/Annotation/PolygonAnnotation.h"
#include "ViewModels/ImageFrameViewModel/Annotation/RectAnnotation.h"
#include "ViewModels/ImageFrameViewModel/Annotation/TextAnnotation.h"


#define IMAGE_INFO_LAYOUT_MARGIN_SPACE 6

ImageDisplayerWidget::ImageDisplayerWidget(QWidget* parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId)
	: ImageDisplayerWidgetBase(parent),
		m_LastMousePosition(0,0)
{
	SetGroupId(messengerOwnerId);

	SetImageViewModel(NULL_SYMBAL);

	//SetBackgroundBrush(Qt::white);
	SetBackgroundBrush(CThemeDefine::GetInstance().GetImageBackground());

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	SetIsMousePress(false);

	SetMouseAction(ImageMouseAction::UnkownAction);

	SetLastClientRectHeight(0);
	SetLastClientRectWidth(0);

	GetSelectedBound().resetTransform();
	GetSelectedBound().setZValue(1);
	GetSelectedBound().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetSelectedBound().setBrush(Qt::transparent);
	GetSelectedBound().setPen(
		CThemeDefine::GetInstance().GetSelectedForground());

	GetActivatedBound().resetTransform();
	GetActivatedBound().setZValue(1);
	GetActivatedBound().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetActivatedBound().setBrush(Qt::transparent);
	GetActivatedBound().setPen(
		QPen(QBrush(
		CThemeDefine::GetInstance().GetActivatedForground()),
			1, Qt::DashDotLine));
	
	GetGraphicsScene().addItem(&GetSelectedBound());
	GetGraphicsScene().addItem(&GetActivatedBound());
	
	GetSelectedBound().setVisible(false);
	GetActivatedBound().setVisible(false);

	auto size = QRect(0,0, 100, 100);
	InitializeImageInfo(size);

	SetIsSelected(false);
	SetIsActive(false);
}

void ImageDisplayerWidget::SetImageViewModel(std::shared_ptr<CDisplayImage> imageViewModel)
{
	ClearAnnotations();
	
	m_ImageViewModel = imageViewModel;

	UpdateFromViewModel();
}

void ImageDisplayerWidget::Reset()
{
	if ( !GetImageViewModel())
		return;

	auto clientSize = rect();

	GetImageViewModel()->ResetImage(clientSize.width(), clientSize.height());

	UpdateQImageByWindowLevel();

	GetGraphicsPixmapItem().resetTransform();

	GetGraphicsPixmapItem().setRotation(0);
		GetGraphicsPixmapItem().setScale(
		GetImageViewModel()->GetImageScale());

	GetGraphicsPixmapItem().setPos(
		GetImageViewModel()->GetTranslateX(),
		GetImageViewModel()->GetTranslateY());

	RelayoutImageInfoText();

	UpdateAnnotations();
}

void ImageDisplayerWidget::resizeEvent(QResizeEvent* event)
{
	auto clientSize = rect();

	if (GetLastClientRectHeight() == clientSize.height()
		&& GetLastClientRectWidth() == clientSize.width())
		return;

	SetLastClientRectHeight(clientSize.height());
	SetLastClientRectWidth(clientSize.width());

	clientSize.adjust(1, 1, -2, -2);

	QRect sizeT(clientSize);

	sizeT.adjust(2, 2, -3, -3);
	
	GetSelectedBound().setRect(sizeT);
	
	sizeT.adjust(2, 2, -3, -3);
	GetActivatedBound().setRect(sizeT);

	GetGraphicsScene().setSceneRect(clientSize);
	setScene(&GetGraphicsScene());

	GetGraphicsScene().update(clientSize);

	if ( !GetImageViewModel())
		return;
	
	GetImageViewModel()->Resize(
		clientSize.width(), clientSize.height());

	GetGraphicsPixmapItem().resetTransform();

	GetGraphicsPixmapItem().setRotation(0);

	GetGraphicsPixmapItem().setPos(
		GetImageViewModel()->GetTranslateX(),
		GetImageViewModel()->GetTranslateY());

	GetGraphicsPixmapItem().setScale(
		GetImageViewModel()->GetImageScale());

	RelayoutImageInfoText();

	UpdateAnnotations();
}

void ImageDisplayerWidget::wheelEvent(QWheelEvent* event)
{
	event->accept();
	
	if (!GetImageViewModel())
		return;

	ClearAnnotations();
	GetImageViewModel()->SwitchToNextFrame(event->delta() > 0);

	UpdateQImageByWindowLevel();

	RelayoutImageInfoText();

	UpdateAnnotations();
}

QPointF ImageDisplayerWidget::ConvertToImageCoordinate(QMouseEvent *ev)
{
	return GetGraphicsPixmapItem().mapFromScene(mapToScene(ev->pos()));
}

void ImageDisplayerWidget::mouseMoveEvent(QMouseEvent * ev)
{
	ev->accept();

	const auto posInImage = ConvertToImageCoordinate(ev);

	if (GetEditingAnnotation())
	{
		GetEditingAnnotation()->OnMouseMoving(
			posInImage.x(), posInImage.y());

		auto resultPos = ComputeAnnotationResultTextPosition(
			GetEditingAnnotation().get());

		GetEditingAnnotation()->UpdatePositions(
			GetImageViewModel()->GetImageScale(),
			GetImageViewModel()->GetTranslateX(),
			GetImageViewModel()->GetTranslateY(),
			resultPos);
		
		//update();
		
		return;
	}

	if ( !GetImageViewModel())
		return;

	for(auto &it : GetImageViewModel()->GetAnnotations())
	{
		it->OnMouseMoving(posInImage.x(), posInImage.y());
	}
	
	if (!GetIsMousePress())
		return;

	auto currentPos = mapToScene(mapFromParent(ev->pos()));

	auto clientRect = rect();

	auto offsetX = currentPos.x() - GetLastMousePosition().x();
	auto offsetY = currentPos.y() - GetLastMousePosition().y();

	bool hasChanged = false;
	
	switch(GetMouseAction())
	{
	case ImageMouseAction::Scale:
		{
			if (abs(offsetX) > 1 || abs(offsetY) > 1)
			{
				auto scale = -offsetY / (clientRect.height() / 2.0);

				if (abs(scale) > 0.001)
				{
					if (GetImageViewModel()
						&& !GetImageViewModel()->IsScaleOverSize(scale))
					{
						GetImageViewModel()->Scale(scale);

						if(clientRect.width() > clientRect.height())
						{
							GetImageViewModel()->TranslateImage(
							-clientRect.height() * scale / 2,
							-clientRect.height() * scale / 2);
						}
						else
						{
							GetImageViewModel()->TranslateImage(
								-clientRect.width() * scale / 2,
								-clientRect.width() * scale / 2);
						}
						
						GetGraphicsPixmapItem().setPos(
							GetImageViewModel()->GetTranslateX(),
							GetImageViewModel()->GetTranslateY());
						
						GetGraphicsPixmapItem().setScale(
							GetImageViewModel()->GetImageScale());

						hasChanged = true;
					}
				}
				
				SetLastMousePosition(currentPos);
			}
		}
		break;

	case ImageMouseAction::Translate:
		{
			if (abs(offsetX) > 1 || abs(offsetY) > 1)
			{
				if (GetImageViewModel())
				{
					GetImageViewModel()->TranslateImage(offsetX, offsetY);

					GetGraphicsPixmapItem().setPos(
						GetImageViewModel()->GetTranslateX(),
						GetImageViewModel()->GetTranslateY());

					hasChanged = true;
				}

				SetLastMousePosition(currentPos);
			}
		}
		break;

	case ImageMouseAction::Rotate:
		break;

	case ImageMouseAction::WindowLevel:
		{
			if (abs(offsetX) > 1 || abs(offsetY) > 1)
			{
				if (GetImageViewModel())
				{
					auto offsetWindowCenter = offsetX * (2048.0 / clientRect.width());
					auto offsetWindowWidth = offsetY * (2048.0 / clientRect.height());

					GetImageViewModel()->WindowLevel(offsetWindowCenter, offsetWindowWidth);

					UpdateQImageByWindowLevel();

					hasChanged = true;
				}

				SetLastMousePosition(currentPos);
			}
		}
		break;
	}

	if(hasChanged)
	{
		RelayoutImageInfoText();

		UpdateAnnotations();
	}
}

void ImageDisplayerWidget::mousePressEvent(QMouseEvent * ev)
{
	ev->accept();

	SetIsMousePress(true);

	SetLastMousePosition(
		mapToScene(mapFromParent(ev->pos())));

	if (GetImageViewModel())
	{
		GetImageViewModel()->SetIsSelected(true);

		GetImageViewModel()->SetIsActive(true);

		CSelectedImageChangedEventArgs eventArgs;

		eventArgs.SetActivadeTarget(this);
		eventArgs.SetIsMultiSelectMode(
			QApplication::keyboardModifiers() == Qt::ControlModifier);

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::SelectImageChanged,
			this,
			&eventArgs);

		const auto posInImage = ConvertToImageCoordinate(ev);

		for(auto &it : GetImageViewModel()->GetAnnotations())
		{
			if (it->OnMouseLeftButtonDown(posInImage.x(), posInImage.y()))
			{
				SetEditingAnnotation(it);
				
				return;
			}
		}

		auto isCreateAnnotation = GetEditingAnnotation() == NULL_SYMBAL;
		
		if(GetEditingAnnotation() == NULL_SYMBAL)
		{
			const auto action = GetMouseAction();
			if (action == ImageMouseAction::AnnotationLine)
				SetEditingAnnotation(std::make_shared<CLineAnnotation>());
			else if (action == ImageMouseAction::AnnotationAngle)
				SetEditingAnnotation(std::make_shared<CAngleAnnotation>());
			else if (action == ImageMouseAction::AnnotationCurve)
				SetEditingAnnotation(std::make_shared<CCurveLineAnnotation>());
			else if (action == ImageMouseAction::AnnotationPolygon)
				SetEditingAnnotation(std::make_shared<CPolygonAnnotation>());
			else if (action == ImageMouseAction::AnnotationText)
			{
				auto ann = std::make_shared<CTextAnnotation>();
				
				auto annText = CreateTemplateEditControl(ev->pos());

				QByteArray bytes = annText.toLocal8Bit();

				ann->SetInputText(
					bytes.data()/*annText.toStdString()*/,
					posInImage,
					GetGraphicsPixmapItem().mapToScene(posInImage));
				
				SetEditingAnnotation(ann);
			}
			else if (action == ImageMouseAction::AnnotationRect)
				SetEditingAnnotation(std::make_shared<CRectAnnotation>());
		}
		
		if (GetEditingAnnotation())
		{
			if(isCreateAnnotation)
			{
				GetEditingAnnotation()->AddToScene(&GetGraphicsScene());

				GetImageViewModel()->AddAnnotation(GetEditingAnnotation());
			}
			
			GetEditingAnnotation()->OnMouseLeftButtonDown(
				posInImage.x(), posInImage.y());

			if(GetEditingAnnotation()->GetAnnotationType()
				== AnnotationType::TextAnnotation)
			{
				SetEditingAnnotation(NULL_SYMBAL);
				//GetEditingAnnotation().reset();
			}
		}
	}
}

void ImageDisplayerWidget::SetIsActive(bool bValue)
{
	if (GetImageViewModel())
		GetImageViewModel()->SetIsActive(bValue);

	m_IsActive = bValue;

	GetActivatedBound().setVisible(m_IsActive);
}

void ImageDisplayerWidget::DeleteLastAnnotation()
{
	if (!GetImageViewModel())
		return;

	ClearAnnotations();

	GetImageViewModel()->RemoveAnnotation();
	
	UpdateAnnotations();
}

void ImageDisplayerWidget::SetIsSelected(bool bValue)
{
	if (GetImageViewModel())
		GetImageViewModel()->SetIsSelected(bValue);

	m_IsSelected = bValue;

	GetSelectedBound().setVisible(m_IsSelected);
}

void ImageDisplayerWidget::mouseReleaseEvent(QMouseEvent * ev)
{
	ev->accept();

	SetIsMousePress(false);

	if (GetEditingAnnotation())
	{
		const auto posInImage = ConvertToImageCoordinate(ev);

		GetEditingAnnotation()->OnMouseLeftButtonUp(
			posInImage.x(), posInImage.y());

		if(GetEditingAnnotation()->GetIsCompleted())
		{
			//GetImageViewModel()->AddAnnotation(GetEditingAnnotation());

			auto resultPos = ComputeAnnotationResultTextPosition(
				GetEditingAnnotation().get());
			
			GetEditingAnnotation()->UpdatePositions(
				GetImageViewModel()->GetImageScale(),
				GetImageViewModel()->GetTranslateX(),
				GetImageViewModel()->GetTranslateY(),
				resultPos);
			
			SetEditingAnnotation(NULL_SYMBAL);
		}
	}
}

void ImageDisplayerWidget::UpdateFromViewModel()
{
	CreateQImage();

	UpdateAnnotations();
	
	update();
}


void ImageDisplayerWidget::RelayoutImageInfoText()
{
	auto clientSize = rect();
	
	GetTopLeftTextItem().setPlainText(GetImageViewModel()->GetFormatedTopLeftText());
	GetTopRightTextItem().setPlainText(GetImageViewModel()->GetFormatedTopRightText());
	GetBottomLeftTextItem().setPlainText(GetImageViewModel()->GetFormatedBottomLeftText());
	GetBottomRightTextItem().setPlainText(GetImageViewModel()->GetFormatedBottomRightText());

	GetTopRightTextItem().setTextWidth(
		max(clientSize.width() / 2, 
			GetTopRightTextItem().document()->idealWidth()));

	GetTopRightTextItem().setPos(
		clientSize.right() - IMAGE_INFO_LAYOUT_MARGIN_SPACE - GetTopRightTextItem().textWidth(),
		clientSize.top() + IMAGE_INFO_LAYOUT_MARGIN_SPACE);

	GetBottomLeftTextItem().setTextWidth(
		GetBottomLeftTextItem().document()->idealWidth() * 2);

	GetBottomLeftTextItem().setPos(
		max(0, clientSize.left() + IMAGE_INFO_LAYOUT_MARGIN_SPACE),
		max(0, clientSize.bottom() - IMAGE_INFO_LAYOUT_MARGIN_SPACE - 18 * 3));

	GetBottomRightTextItem().setTextWidth(
		clientSize.width() / 2 - 2);

	GetBottomRightTextItem().setPos(
		max(0, clientSize.width() / 2),
		max(0, clientSize.bottom() - IMAGE_INFO_LAYOUT_MARGIN_SPACE - 18 * 4));
}

void ImageDisplayerWidget::CreateQImage()
{
	if (!GetImageViewModel())
		return;

	auto qimage = GetImageViewModel()->GetQImage();

	if (qimage == NULL_SYMBAL)
		return;
	
	SetQPixmap(QPixmap::fromImage(*qimage));

	auto clientSize = rect();

	GetImageViewModel()->Resize(
		clientSize.width(),
		clientSize.height());
	
	GetGraphicsPixmapItem().setPixmap(GetQPixmap());
	
	GetGraphicsPixmapItem().resetTransform();
	
	GetGraphicsPixmapItem().setRotation(0);
	GetGraphicsPixmapItem().setScale(
		GetImageViewModel()->GetImageScale());
	
	GetGraphicsPixmapItem().setPos(
		GetImageViewModel()->GetTranslateX(),
		GetImageViewModel()->GetTranslateY());

	GetGraphicsPixmapItem().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetGraphicsScene().addItem(&GetGraphicsPixmapItem());

	InitializeImageInfo(clientSize);

	GetGraphicsScene().setSceneRect(clientSize);
	setScene(&GetGraphicsScene());
	
	GetGraphicsScene().update(clientSize);
}

void ImageDisplayerWidget::UpdateQImageByWindowLevel()
{
	if (!GetImageViewModel())
		return;

	auto qimage = GetImageViewModel()->GetQImage();

	if (qimage == NULL_SYMBAL)
		return;

	SetQPixmap(QPixmap::fromImage(*qimage));
	GetGraphicsPixmapItem().setPixmap(GetQPixmap());
}

void ImageDisplayerWidget::InitializeImageInfo(QRect& clientSize)
{
	auto centerX = clientSize.width() / 2 - 10;
	auto centerY = clientSize.height() / 2 - 10;

	GetTopLeftTextItem().resetTransform();
	GetTopLeftTextItem().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetTopLeftTextItem().setPos(
		clientSize.left() + IMAGE_INFO_LAYOUT_MARGIN_SPACE,
		clientSize.top() + IMAGE_INFO_LAYOUT_MARGIN_SPACE);
	GetTopLeftTextItem().setFont(CThemeDefine::GetInstance().GetImageInfoFont());
	GetTopLeftTextItem().setDefaultTextColor(CThemeDefine::GetInstance().GetImageInfoForground());
	GetTopLeftTextItem().setZValue(1);
	GetGraphicsScene().addItem(&GetTopLeftTextItem());

	GetTopRightTextItem().resetTransform();
	GetTopRightTextItem().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetTopRightTextItem().setPos(
		clientSize.right()- IMAGE_INFO_LAYOUT_MARGIN_SPACE,
		clientSize.top() + IMAGE_INFO_LAYOUT_MARGIN_SPACE);
	GetTopRightTextItem().setFont(CThemeDefine::GetInstance().GetImageInfoFont());
	GetTopRightTextItem().setDefaultTextColor(CThemeDefine::GetInstance().GetImageInfoForground());

	{
		auto document = GetTopRightTextItem().document();
		auto option = document->defaultTextOption();
		option.setAlignment(Qt::AlignRight | Qt::AlignTop);
		document->setDefaultTextOption(option);
		GetTopRightTextItem().setTextWidth(document->idealWidth());
	}

	GetTopRightTextItem().setZValue(1);
	GetGraphicsScene().addItem(&GetTopRightTextItem());

	GetBottomLeftTextItem().resetTransform();
	GetBottomLeftTextItem().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetBottomLeftTextItem().setPos(
		clientSize.left() + IMAGE_INFO_LAYOUT_MARGIN_SPACE,
		clientSize.bottom() - IMAGE_INFO_LAYOUT_MARGIN_SPACE);
	GetBottomLeftTextItem().setFont(CThemeDefine::GetInstance().GetImageInfoFont());
	GetBottomLeftTextItem().setDefaultTextColor(CThemeDefine::GetInstance().GetImageInfoForground());
	{
		auto document = GetBottomLeftTextItem().document();
		auto option = document->defaultTextOption();
		option.setAlignment(Qt::AlignLeft | Qt::AlignBottom);
		document->setDefaultTextOption(option);
		GetBottomLeftTextItem().setTextWidth(document->idealWidth());
	}
	GetBottomLeftTextItem().setZValue(1);
	GetGraphicsScene().addItem(&GetBottomLeftTextItem());

	GetBottomRightTextItem().resetTransform();
	GetBottomRightTextItem().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	GetBottomRightTextItem().setPos( 
		clientSize.right() - IMAGE_INFO_LAYOUT_MARGIN_SPACE,
		clientSize.bottom() - IMAGE_INFO_LAYOUT_MARGIN_SPACE);
	GetBottomRightTextItem().setFont(CThemeDefine::GetInstance().GetImageInfoFont());
	GetBottomRightTextItem().setDefaultTextColor(CThemeDefine::GetInstance().GetImageInfoForground());

	{
		auto document = GetBottomRightTextItem().document();
		auto option = document->defaultTextOption();
		option.setAlignment(Qt::AlignRight | Qt::AlignBottom);
		document->setDefaultTextOption(option);
		GetBottomRightTextItem().setTextWidth(document->idealWidth());
	}

	GetBottomRightTextItem().setZValue(1);
	GetGraphicsScene().addItem(&GetBottomRightTextItem());
}

QString ImageDisplayerWidget::CreateTemplateEditControl(QPoint position)
{
	FloatInputTextWindow window;
	window.SetDiplayLocation(mapToGlobal(mapFromScene(position)));
	
	window.exec();

	return window.GetInputTextValue();
}

void ImageDisplayerWidget::ClearAnnotations()
{
	if (!GetImageViewModel())
		return;

	for(auto &it : GetImageViewModel()->GetAnnotations())
	{
		it->RemoveFromScene(&GetGraphicsScene());
	}
}

void ImageDisplayerWidget::UpdateAnnotations()
{
	if (!GetImageViewModel())
		return;

	auto scale = GetImageViewModel()->GetImageScale();
	auto translateX = GetImageViewModel()->GetTranslateX();
	auto translateY = GetImageViewModel()->GetTranslateY();
	
	for (auto &it : GetImageViewModel()->GetAnnotations())
	{
		it->AddToScene(&GetGraphicsScene());
		
		auto resultPos =
			ComputeAnnotationResultTextPosition(it.get());

		it->UpdatePositions(scale, translateX, translateY, resultPos);
		
	}
}

QPointF ImageDisplayerWidget::ComputeAnnotationResultTextPosition(CAnnotationBase *pAnn)
{
	static QPointF EMPTY_POINT(0, 0);
	
	if (pAnn == NULL_SYMBAL)
		return EMPTY_POINT;

	return GetGraphicsPixmapItem().mapToScene(
		pAnn->GetSuggestResultTextPosition());
}

