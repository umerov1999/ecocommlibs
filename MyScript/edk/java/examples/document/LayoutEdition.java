package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.FontData;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.LayerIterator;
import com.myscript.document.Layout;
import com.myscript.document.LayoutItemData;
import com.myscript.document.LayoutItemIterator;
import com.myscript.document.LayoutObjectData;
import com.myscript.document.LayoutStrokesData;
import com.myscript.document.Page;
import com.myscript.document.PenData;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Rectangle;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.ink.InkStrokeBuilder;
import com.myscript.ink.InkStrokeFormat;
import com.myscript.ink.InkStrokeFormatBuilder;
import common.CFG;

/**
 * The <code>LayoutEdition/code> example code shows you how to add images and
 * ink strokes to the layout of a page.
 */
public class LayoutEdition
{
  /**
   * The working folder and document files used in this example. (Note that you
   * can rename any notes document as a zip file to watch inside it.)
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  private static final String DOCUMENT_FILE       = CFG.DATA_DIR + "/example4.notes";
  private static final String IMAGE_FILE          = CFG.DATA_DIR + "/image.png";

  private static final float[][] RECTANGLE =
    {
      // x, y
      { 2869.18f, 1977.23f },
      { 2835.84f, 1977.23f },
      { 2780.29f, 1961.89f },
      { 2702.52f, 1938.87f },
      { 2580.29f, 1908.19f },
      { 2446.97f, 1869.83f },
      { 2291.42f, 1839.14f },
      { 2135.87f, 1816.13f },
      { 1969.21f, 1808.45f },
      { 1824.77f, 1800.79f },
      { 1702.55f, 1800.79f },
      { 1602.55f, 1793.11f },
      { 1524.78f, 1785.44f },
      { 1469.23f, 1777.77f },
      { 1435.9f, 1762.43f },
      { 1435.9f, 1754.75f },
      { 1435.9f, 1747.08f }
    };

  private static final float[][] CIRCLE =
    {
      // x, y
      { 6110.47f, 1818.18f },
      { 6029.92f, 1760.64f },
      { 5972.38f, 1737.63f },
      { 5903.34f, 1726.12f },
      { 5822.78f, 1726.12f },
      { 5742.23f, 1737.63f },
      { 5650.17f, 1783.66f },
      { 5569.62f, 1852.7f },
      { 5489.07f, 1944.76f },
      { 5408.52f, 2059.84f },
      { 5350.98f, 2186.42f },
      { 5293.44f, 2336.02f },
      { 5258.92f, 2474.11f },
      { 5258.92f, 2612.2f },
      { 5281.93f, 2738.78f },
      { 5362.49f, 2853.85f },
      { 5466.05f, 2945.91f },
      { 5604.14f, 3014.96f },
      { 5765.25f, 3049.48f },
      { 5914.84f, 3060.99f },
      { 6075.95f, 3037.97f },
      { 6214.04f, 2968.93f },
      { 6340.62f, 2876.87f },
      { 6432.68f, 2738.78f },
      { 6501.73f, 2577.68f },
      { 6524.74f, 2393.56f },
      { 6524.74f, 2209.44f },
      { 6501.73f, 2059.84f },
      { 6432.68f, 1944.76f },
      { 6352.13f, 1852.7f },
      { 6248.56f, 1795.17f },
      { 6156.5f, 1760.64f },
      { 6064.44f, 1749.14f },
      { 5983.89f, 1749.14f },
      { 5903.34f, 1772.15f }
    };

  
  /**
   * Main entry point for the <code>LayoutEdition</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of LayoutEdition class");
      System.exit(-1);
    }

    if (IMAGE_FILE == null || IMAGE_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for image file. You can choose image filename by editing IMAGE_FILE static member variable of LayoutEdition class");
      System.exit(-1);
    }

    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of LayoutEdition class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document");
    final Document document = (Document)EngineObject.load(engine, DOCUMENT_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();
    
    System.out.println("3) retrieving the first Page of the Document");
    int pageCount = document.getPageCount();
    if (pageCount <= 0)
    {
      System.err.println(" . the Document doesn't contain any page");
      System.exit(-1);
    }
    final Page page = document.getPageAt(0);
    System.out.println(" . Page retrieved successfully");
    System.out.println();

    System.out.println("4) retrieving the Layout and Ink of the Page");
    System.out.println(" . sending the Layout request to the Page");
    final Layout[] layoutHolder = new Layout[1];
    final Ink[] inkHolder = new Ink[1];
    page.getLayout(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to get the Layout of the Page");
        else
        {  
          layoutHolder[0] = (Layout)(value.acquire());
        }
      }
    });
    System.out.println(" . sending the Ink request to the Page");
    page.getInk(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to get the Ink of the Page");
        else
        {
          inkHolder[0] = (Ink)(value.acquire());
        }
      }
    });
    System.out.println(" . waiting for the requests to complete");
    System.out.println("   . note that sync() should be called only if you need the pending");
    System.out.println("     requests to be completed before any other operation is performed");
    page.sync();
    final Layout layout = layoutHolder[0];
    final Ink ink = inkHolder[0];
    if ((layout == null) || (ink == null))
    {
      System.err.println(" . failed to retrieve the Layout and/or the Ink");
      System.exit(-1);
    }
    System.out.println(" . Layout and Ink retrieved successfully");
    System.out.println();

    System.out.println("5) adding a Layer to the Layout");
    int layerCount = layout.getLayerCount();
    final String layerName = "newLayer";
    layout.addLayer(layerName);
    if (layout.getLayerCount() != (layerCount + 1))
    {
      System.err.println(" . impossible to add a new Layer");
      System.exit(-1);
    }
    System.out.println(" . new Layer added successfully");
    System.out.println();

    System.out.println("6) adding a new Pen and Font to the Layout");
    final PenData pen = new PenData("CalligraphicBrush", "None", 1.0f, 0x15A607FF);
    final String penId = "newPen";
    layout.addPen(penId, pen);
    System.out.println(" . new Pen added successfully");
    final FontData font = new FontData("courier", 400.0f, 12.0f, 1.2f, "normal", "normal");
    final String fontId = "newFont";
    layout.addFont(fontId, font);
    System.out.println(" . new Font added successfully");
    System.out.println();
    System.out.println();

    System.out.println("7) adding a LayoutItem of type strokes");
    System.out.println(" . creating a Tag to tag the ink that will be associated");
    System.out.println("   with the new LayoutItem");
    final String tagName = "strokesLayoutItem";
    final long tagId = ink.addTag(tagName, null, null);
    if (tagId == -1)
    {
      System.err.println(" . impossible to create a new tag " + tagName);
      System.exit(-1);
    }
    System.out.println(" . retrieving an iterator over the Layers");
    final LayerIterator layerIterator = layout.getLayer(layerName);
    if (layerIterator.isAtEnd())
    {
      System.err.println(" . the Layout doesn't contain a Layer \"" + layerName + "\"");
      System.exit(-1);
    }
    System.out.println(" . adding the LayoutItem to the Layer");
    final LayoutStrokesData strokeItem = new LayoutStrokesData(tagId, penId, fontId);
    layerIterator.addItem(strokeItem);
    System.out.println(" . LayoutItem added successfully");
    System.out.println();

    System.out.println("8) creating an InkStrokeBuilder");
    System.out.println(" . creating an InkStrokeFormatBuilder");
    final InkStrokeFormatBuilder strokeFormatBuilder = InkStrokeFormatBuilder.create(engine);
    System.out.println(" . specifying the format content: 2 channels X and Y");
    final String xChannelName = "X";
    final String yChannelName = "Y";
    strokeFormatBuilder.addChannel(xChannelName);
    strokeFormatBuilder.addChannel(yChannelName);
    System.out.println(" . creating the InkStrokeFormat");
    final InkStrokeFormat strokeFormat = strokeFormatBuilder.createStrokeFormat();
    System.out.println(" . creating an InkStrokeBuilder with the InkStrokeFormat");
    final InkStrokeBuilder strokeBuilder = InkStrokeBuilder.create(strokeFormat);
    System.out.println(" . InkStrokeBuilder created successfully");
    System.out.println();

    System.out.println("9) creating an InkStroke");
    System.out.println(" . adding the InkStroke samples to the InkStrokeBuilder");
    for (int sampleIndex = 0; sampleIndex < RECTANGLE.length; ++sampleIndex)
    {
      strokeBuilder.addSample(RECTANGLE[sampleIndex]);
    }

    System.out.println(" . retrieving the InkStroke from the InkStrokeBuilder");
    final InkStroke stroke_1 = strokeBuilder.createStroke();
    System.out.println(" . InkStroke created successfully");
    System.out.println();

    final long tagIdHolder[] = { tagId }; 

    System.out.println("10) adding the InkStroke to the previously created LayoutItem (through its Tag id)");
    ink.addStroke(stroke_1, tagIdHolder);
    System.out.println(" . InkStroke added successfully");
    System.out.println();

    System.out.println("11) creating another InkStroke and adding it to the same LayoutItem");
    System.out.println(" . clearing the InkStrokeBuilder");
    strokeBuilder.clear();
    System.out.println(" . adding the stroke samples to the InkStrokeBuilder");
    for (int sampleIndex = 0; sampleIndex < CIRCLE.length; ++sampleIndex)
    {
      strokeBuilder.addSample(CIRCLE[sampleIndex]);
    }

    System.out.println(" . retrieving the InkStroke from the InkStrokeBuilder");
    final InkStroke stroke_2 = strokeBuilder.createStroke();
    System.out.println(" . adding the InkStroke to the previously created LayoutItem (through its Tag id)");
    ink.addStroke(stroke_2, tagIdHolder);
    System.out.println(" . InkStroke added successfully");
    System.out.println();

    System.out.println("12) creating a LayoutItem containing an image");
    System.out.println(" . adding an image to the document");
    final String imageUrl = "exampleImage.png";
    document.addObject(imageUrl, IMAGE_FILE, new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to add Object to the Document");
        else
          System.out.println("   . Object added successfully");
      }
    });
    System.out.println(" . waiting for the image addition to complete successfully");
    System.out.println("   . note that sync() should be called only if you need the pending");
    System.out.println("     requests to be completed before any other operation is performed");
    document.sync();
    System.out.println(" . adding a new LayoutItem referencing the image");
    final LayoutObjectData imageItem = new LayoutObjectData(imageUrl, new Rectangle(1000.0f, 0.0f, 1000.0f, 1500.0f), null, "image/png");
    layerIterator.addItem(imageItem);
    System.out.println(" . LayoutItem added successfully");
    System.out.println();
    
    System.out.println("13) displaying Layout description");
    {
      layerCount = layout.getLayerCount();
      System.out.println(" . retrieving number of Layers = " + layerCount);
      System.out.println(" . iterating on all Layers");

      final LayerIterator layerIterator2 = layout.getLayers();
      while(layerIterator2.isAtEnd() == false)
      {
        String currentLayerName = null;
        int layoutItemCount = -1;
        LayoutItemIterator layoutItemIterator = null;

        currentLayerName = layerIterator2.getId();
        System.out.println("   . retrieving Layer id = \"" + currentLayerName + "\"");

        layoutItemCount = layerIterator2.getItemCount();
        if (layoutItemCount < 0)
        {
          System.err.println(" . the Layer doesn't contain any Layout items");
          System.exit(-1);
        }
        System.out.println("     . retrieving number of Layout Items in the Layer = " + layoutItemCount);

        if (layoutItemCount > 0)
        {
          System.out.println("     . iterating on Layout Items");
          layoutItemIterator = layerIterator2.getItems();
          while(layoutItemIterator.isAtEnd() == false)
          {
            final LayoutItemData itemData = layoutItemIterator.getData();

            if (itemData instanceof LayoutStrokesData)
            {
              final LayoutStrokesData data = (LayoutStrokesData)(itemData);

              System.out.println("       item : LayoutStrokesData");
              System.out.println("       . pen Id = \"" + data.getPenId() + "\"");
              System.out.println("       . font Id = \"" + data.getFontId() + "\"");
              System.out.println("       . tag Id = " + data.getTagId());
            }
            else if (itemData instanceof LayoutObjectData)
            {
              final LayoutObjectData data = (LayoutObjectData)(itemData);
              final Rectangle rectangle = data.getRectangle();

              System.out.println("       item : LayoutObjectData");
              System.out.println("       . url = \"" + data.getUrl() + "\"");
              System.out.println("       . rectangle = { " + rectangle.x + ", " + rectangle.y + ", " + rectangle.width + ", " + rectangle.height + " }");
              if (data.getTransformId() != null)
                System.out.println("       . transform = " + data.getTransformId());
              if (data.getMimeType() != null)
                System.out.println("       . mime type = \"" + data.getMimeType() + "\"");
            }
        
            layoutItemIterator.next();
          }
          System.out.println("     . releasing Layout Items iterator");
          layoutItemIterator.dispose();
        }
        layerIterator2.next();
      }
      System.out.println("   . releasing Layer iterator");
      layerIterator2.dispose();
      System.out.println(" . Layout description displayed successfully");
    }
    System.out.println();


    System.out.println("14) requesting to discard the modifications performed on the Document");
    document.discardChanges(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to discard the changes of the document");
        else
          System.out.println(" . changes discarded successfully (asynchronous operation requested at step 14)");
      }
    });
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("15) cleaning-up: disposing of objects and disposing of the Engine");
    ink.dispose();
    layout.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to manipulate the Layout of a Page");
  }

} // LayoutEdition
