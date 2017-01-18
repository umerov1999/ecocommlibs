package engine;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.FloatStructuredInput;

/**
 * The <code>ObjectsRelationships</code> example code shows you the
 * relationships that exist between an <code>Engine</code> and a child
 * <code>EngineObject</code>.
 */
public class ObjectsRelationships
{
  /**
   * Main entry point for the <code>ObjectsRelationships</code> example program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) creating a new FloatStructuredInput");
    final FloatStructuredInput input1 = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println(" . right after its creation, the status of the FloatStructuredInput is:");
    System.out.println("   . is the FloatStructuredInput disposed of = " + input1.isDisposed());
    System.out.println();
    
    System.out.println("3) do not forget the 1st rule: you create it, you dispose of it!");
    System.out.println(" . disposing of the FloatStructuredInput");
    input1.dispose();
    System.out.println(" . is the FloatStructuredInput disposed of = " + input1.isDisposed());
    System.out.println();
    
    System.out.println("4) creating another FloatStructuredInput");
    final FloatStructuredInput input2 = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println(" . right after its creation, the status of the FloatStructuredInput is:");
    System.out.println("   . is the FloatStructuredInput disposed of = " + input2.isDisposed());
    System.out.println();
    
    System.out.println("5) do not forget the 2nd rule: disposing of a parent disposes of the children!");
    System.out.println(" . disposing of the Engine");
    engine.dispose();
    System.out.println(" . is the Engine disposed of = " + engine.isDisposed());
    System.out.println(" . disposing of the Engine also disposes of the FloatStructuredInput:");
    System.out.println(" . is the FloatStructuredInput disposed of = " + input2.isDisposed());
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt the relationships between Engine and EngineObject");
  }
  
} // ObjectsRelationships
