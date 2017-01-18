package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Layout;
import com.myscript.document.Page;
import com.myscript.document.PageSelection;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.HistoryManager;
import com.myscript.geometry.Rectangle;
import com.myscript.geometry.Transform;
import com.myscript.ink.SelectionModifier;
import common.CFG;

/**
 * The <code>UndoRedo/code> example code shows you how to undo or redo actions
 * performed on a document.
 */
public class UndoRedo
{
  /**
   * The working folder and document files used in this example.
   * (Note that you can rename any notes document as a zip file to watch inside
   * it.)
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  private static final String DOCUMENT_FILE       = CFG.DATA_DIR + "/example4.notes";

  
  /**
   * Main entry point for the <code>UndoRedo</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of UndoRedo class");
      System.exit(-1);
    }

    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of UndoRedo class");
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

    System.out.println("4) retrieving the Layout of the Page");
    System.out.println(" . sending the Layout request to the Page");
    final Layout[] layoutHolder = new Layout[1];
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
    System.out.println(" . waiting for the Layout request to complete");
    System.out.println("   . note that sync() should be called only if you need the Layout");
    System.out.println("     immediately and if you have nothing else to do in the meantime");
    page.sync();
    final Layout layout = layoutHolder[0];
    if (layout == null)
    {
      System.err.println(" . failed to retrieve the Layout");
      System.exit(-1);
    }
    System.out.println("   . Layout retrieved successfully");
    System.out.println();

    System.out.println("5) retrieving the Document HistoryManager");
    final HistoryManager historyManager = document.getHistoryManager();
    System.out.println(" . HistoryManager retrieved successfully");
    System.out.println();

    System.out.println("6) creating a PageSelection");
    final PageSelection selection = PageSelection.create(page);
    System.out.println(" . PageSelection created successfully");
    System.out.println();

    System.out.println("7) selecting a portion of the Page");
    final Rectangle rectangle = new Rectangle(32.0f, 122.0f, 300.0f, 500.0f);
    selection.selectRectangle(rectangle, SelectionModifier.SET);
    System.out.println(" . selection performed successfully");
    System.out.println();

    System.out.println("8) erasing selected portion of the Page");
    page.erase(selection);
    System.out.println(" . selection erased successfully");
    System.out.println();

    System.out.println("9) selecting the remaining data in the Page");
    selection.selectAll();
    System.out.println(" . selection performed successfully");
    System.out.println();

    System.out.println("10) creating an horizontal translation transform");
    Transform transform = new Transform();
    transform.setIdentity();
    transform.translate(10.0f, 0.0f);
    System.out.println(" . transform created successfully");
    System.out.println();

    System.out.println("11) translating the selected portion of the Page");
    page.transform(selection, transform);
    System.out.println(" . translation performed successfully");
    System.out.println();

    System.out.println("12) undoing the translation operation");
    boolean canUndo = historyManager.canUndo();
    if (!canUndo)
    {
      System.err.println(" . Can't undo");
      System.exit(-1);
    }
    historyManager.undo();
    System.out.println(" . transform undone successfully");
    System.out.println();

    System.out.println("13) undoing the erasing operation");
    canUndo = historyManager.canUndo();
    if (!canUndo)
    {
      System.err.println(" . Can't undo");
      System.exit(-1);
    }
    historyManager.undo();
    System.out.println(" . erase undone successfully");
    System.out.println();

    System.out.println("14) redoing the erasing operation");
    boolean canRedo = historyManager.canRedo();
    if (!canRedo)
    {
      System.err.println(" . Can't redo");
      System.exit(-1);
    }
    historyManager.redo();
    System.out.println(" . erase redone successfully");
    System.out.println();

    System.out.println("15) starting an HistoryManager transaction, which groups several ");
    System.out.println("    actions so that they can be undone (or redone) with a single call to");
    System.out.println("    undo (or redo)");
    historyManager.startTransaction();
    System.out.println(" . transaction started successfully");
    System.out.println();
    
    System.out.println("16) adding a new Layer to the Layout");
    int layerCount = layout.getLayerCount();
    if (layerCount < 0)
    {
      System.err.println(" . Layout has no layer");
      System.exit(-1);
    }
    final String newLayerId = "newLayer";
    layout.addLayer(newLayerId);
    if (layout.getLayerCount() != (layerCount + 1))
    {
      System.err.println(" . new Layer wasn't added");
      System.exit(-1);
    }
    System.out.println(" . new Layer added successfully");
    System.out.println();

    System.out.println("17) selecting the remaining data in the Page");
    selection.selectAll();
    System.out.println(" . selection performed successfully");
    System.out.println();

    System.out.println("18) generating a vertical translation Transform");
    transform.setIdentity();
    transform.translate(0.0f, 10.0f);
    System.out.println(" . transform created successfully");
    System.out.println();

    System.out.println("19) translating the selected portion of the Page");
    page.transform(selection, transform);
    System.out.println(" . translation performed successfully");
    System.out.println();

    System.out.println("20) committing the HistoryManager transaction");
    historyManager.commit();
    System.out.println(" . transaction commited successfully");
    System.out.println();

    System.out.println("21) undoing the operations performed within the HistoryManager transaction");
    canUndo = historyManager.canUndo();
    if (!canUndo)
    {
      System.err.println(" . Can't undo");
      System.exit(-1);
    }
    historyManager.undo();
    if (layout.getLayerCount() != layerCount)
    {
      System.err.println(" . operations wasn't undone");
      System.exit(-1);
    }
    System.out.println(" . operations undone successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("22) cleaning-up: disposing of objects and disposing of the Engine");
    selection.dispose();
    layout.dispose();
    page.dispose();
    historyManager.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to undo and redo document modifications");
  }

} // UndoRedo
