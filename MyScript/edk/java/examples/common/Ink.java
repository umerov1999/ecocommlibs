package common;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StreamTokenizer;

import com.myscript.engine.IFloatStroke;

public class Ink
{
  public interface IParseHandler
  {
    void addStroke(IFloatStroke stroke);
  }

  public static void parse(File file, IParseHandler handler) throws IOException
  {
    Reader reader = new FileReader(file);
    try
    {
      parse(reader, handler);
    }
    finally
    {
      reader.close();
    }
  }

  public static void parse(Reader reader, IParseHandler handler)
      throws IOException
  {
    StreamTokenizer tokenizer = new StreamTokenizer(reader);
    tokenizer.parseNumbers();
    
    int strokeCount = nextInteger(tokenizer);
    
    for (int strokeIndex = 0; strokeIndex < strokeCount; ++strokeIndex)
    {
      final int pointCount = nextInteger(tokenizer);
      
      final float[] xArray = new float[pointCount];
      final float[] yArray = new float[pointCount];
      
      for (int pointIndex = 0; pointIndex < pointCount; ++pointIndex)
      {
        xArray[pointIndex] = nextFloat(tokenizer);
        yArray[pointIndex] = nextFloat(tokenizer);
      }
      
      handler.addStroke(new IFloatStroke()
      {
        public int getPointCount()
        {
          return pointCount;
        }

        public float getX(int i) throws IndexOutOfBoundsException
        {
          return xArray[i];
        }

        public float getY(int i) throws IndexOutOfBoundsException
        {
          return yArray[i];
        }
      });
    }
  }

  private static int nextInteger(StreamTokenizer tokenizer) throws IOException
  {
    if (tokenizer.nextToken() != StreamTokenizer.TT_NUMBER)
      throw new IOException("parse error: expected integer value");
    int integer = (int) tokenizer.nval;
    if ((double) integer != tokenizer.nval)
      throw new IOException("parse error: expected integer value");
    return integer;
  }

  private static float nextFloat(StreamTokenizer tokenizer) throws IOException
  {
    if (tokenizer.nextToken() != StreamTokenizer.TT_NUMBER)
      throw new IOException("parse error: expected float value");
    float value = (float) tokenizer.nval;
    return value;
  }
}
