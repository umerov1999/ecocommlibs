package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Page;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.json.Json;
import com.myscript.json.JsonType;
import common.CFG;

/**
 * The <code>EditMetadata</code> example code shows you how to edit metadata
 * from a document.
 */
public class EditMetadata
{ 
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its metadata. (Note that you can rename
   * any notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example1.notes";

  /**
   * Main entry point for the <code>EditMetadata</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of EditMetadata class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of EditMetadata class");
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

    System.out.println("3) getting the metadata of the Document");
    final Json metadataHolder[] = new Json[1];
    document.getMetadata(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to retrieve document metadata");
        metadataHolder[0] = (Json)value.acquire();
      }
    });
    document.sync(); // wait for getMetadata() to complete
    final Json documentMetadata = metadataHolder[0];
    System.out.println(" . Document's metadata loaded successfully");
    System.out.println();

    System.out.println("4) setting the author of the Document in its metadata");
    final Json authorValue = Json.createString(engine, "MyScript");
    documentMetadata.putObjectEntry("Author", authorValue);
    authorValue.dispose();
    System.out.println(" . author set successfully");
    System.out.println();

    System.out.println("5) setting the description of the Document in its metadata");
    Json descrValue = Json.createString(engine, "Edition of metadata in a document");
    documentMetadata.putObjectEntry("Description", descrValue);
    descrValue.dispose();
    System.out.println(" . description set successfully");
    System.out.println();

    System.out.println("6) removing the description of the Document in its metadata");
    documentMetadata.removeObjectEntry("Description");
    descrValue = documentMetadata.getObjectEntryValue("Description");
    if (descrValue == null)
      System.out.println(" . description removed successfully");
    System.out.println();

    System.out.println("7) re-setting the first description of the Document in its metadata");
    descrValue = Json.createString(engine, "Edition of metadata in a document");
    documentMetadata.putObjectEntry("Description", descrValue);
    descrValue.dispose();
    System.out.println(" . description set successfully");
    System.out.println();

    System.out.println("8) getting the description of the document from its metadata");
    descrValue = documentMetadata.getObjectEntryValue("Description");
    if (descrValue.getType() == JsonType.STRING)
    {
      System.out.println(" . Document's description = "
          + descrValue.getStringValue());
    }
    descrValue.dispose();
    System.out.println();

    System.out.println("9) getting the metadata of the first page of the Document");
    Page page = document.getPageAt(0);
    page.getMetadata(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to retrieve page metadata");
        metadataHolder[0] = (Json)value.acquire();
      }
    });
    page.sync();
    final Json pageMetadata = metadataHolder[0];
    System.out.println(" . metadata retrieved successfully");
    System.out.println();

    System.out.println("10) setting misc values in the metadata of the first page of the document");
    Json pageValue = Json.createString(engine, "First page of the document");
    pageMetadata.putObjectEntry("Description", pageValue);
    pageValue.dispose();
    System.out.println(" . page description set successfully");

    final Json pageObject = Json.createObject(engine);
    pageMetadata.putObjectEntry("Object", pageObject);
    System.out.println(" . new JSON object added successfully");

    pageValue = Json.createBoolean(engine, true);
    pageMetadata.putObjectEntry("boolean", pageValue);
    pageValue.dispose();
    System.out.println(" . new boolean added successfully");

    System.out.println(" . adding an array");
    final Json pageArray = Json.createArray(engine);
    {
      pageValue = Json.createBoolean(engine, false);
      pageArray.insertArrayValueAt(-1, pageValue);
      pageValue.dispose();
      System.out.println("   . boolean added in the array");

      pageValue = Json.createNumber(engine, 987);
      pageArray.insertArrayValueAt(-1, pageValue);
      pageValue.dispose();
      System.out.println("   . number added in the array");
    }
    pageObject.putObjectEntry("Array", pageArray);
    System.out.println(" . array added successfully");

    pageValue = Json.createNumber(engine, 1234.456);
    pageObject.putObjectEntry("Number", pageValue);
    System.out.println(" . number added successfully");
    pageValue.dispose();

    pageObject.dispose();
    System.out.println(" . metadata set successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("11) cleaning-up: disposing of objects and disposing of the Engine");
    pageMetadata.dispose();
    documentMetadata.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to edit metadata of a document");
  }

} // EditMetadata
