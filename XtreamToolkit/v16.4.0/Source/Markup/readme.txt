Supported Tags

Page               (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight,)
Border             (Cursor, Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Padding, Background, BorderThickness, BorderBrush)
TextBlock          (Cursor, Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Padding, Background, Foreground, TextWrapping, TextAlignment, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle, BaselineAlignment, Text)
Image              (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Source, Stretch )

Run                (Cursor, Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle, BaselineAlignment, Text)
LineBreak
Span             (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)
  Bold             (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)
  Italic           (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)
  Underline        (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)
  InlineUIContainer (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)
  Hyperlink         (Background, Foreground, TextDecorations, FontSize, FontWeight, FontFamily, FontStyle)

Panel              (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Background
  StackPanel       (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Background, Orientation)
  WrapPanel        (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Background, Orientation, ItemHeight, ItemWidth)
  Canvas           (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Background, Left, Right, Top, Bottom)
  Grid             (Style, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, MaxHeight, Background, ColumnDefinitions, RowDefinitions)
    ColumnDefinition (Width, MinWidth, MaxWidth)
    RowDefinition  (Height, MinHeight, MaxHeight)
  UniformGrid      (Rows, Columns, FirstColumn)
  DockPanel        (Dock, LastChildFill)

Polygon            (Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, Points)
Polyline           (Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, Points)
Rectangle          (IsMouseOver, Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight)
Ellipse            (Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight)
Line               (Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, X1, Y1, X2, Y2)
Path               (Data)

Button             (IsPressed, Style, Width, Margin, ClipToBounds, HorizontalAlignment, VerticalAlignment, Width, Height, MinWidth, MaxWidth, MinHeight, Content)
CheckBox
RadioButton

ScrollViewer       (Style, VerticalScrollBarVisibility, HorizontalScrollBarVisibility)

SolidColorBrush    (Color)
LinearGradientBrush  (StartPoint, EndPoint, GradientStops
   GradientStop    (Color, Offset)

Style              (TargetType, BasedOn, Resources)
  Setter	   (Property, Value)


Events - MouseEnter, MouseLeave, MouseMove, MouseLeftButtonUp, MouseLeftButtonDown, MouseRightButtonUp, MouseRightButtonDown