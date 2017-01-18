package text;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.InputUnit;
import com.myscript.text.DigitalInkTagger;
import com.myscript.text.Resource;
import com.myscript.text.TagIterator;
import com.myscript.text.TagList;
import common.CFG;

/**
 * The <code>DigitalInkTagging</code> example code shows you how to perform
 * digital ink tagging.
 */
public class DigitalInkTagging
{
  /**
   * Main entry point for the <code>DigitalInkTagging</code> example
   * program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new DigitalInkTagger");
    final DigitalInkTagger tagger = DigitalInkTagger.create(engine);
    System.out.println(" . DigitalInkTagger created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the DigitalInkTagger");
    System.out.println(" . loading the IsolatedAlphabetKnowledge resource");
    final Resource akIso = (Resource)EngineObject.load(engine, CFG.getAkIsoRes());
    System.out.println(" . IsolatedAlphabetKnowledge resource loaded successfully");

    System.out.println(" . attaching the IsolatedAlphabetKnowledge resource to the DigitalInkTagger");
    tagger.attach(akIso);
    System.out.println(" . IsolatedAlphabetKnowledge resource attached successfully");
    
    System.out.println(" . disposing of the IsolatedAlphabetKnowledge resource (not needed anymore)");
    akIso.dispose();
    System.out.println("   . IsolatedAlphabetKnowledge resource disposed of successfully");
    System.out.println();
    
    System.out.println("4) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("5) fueling the input with strokes written in a boxed field");

    // for each box
    for (int i = 0; i < DIGITAL_INK.length; ++i)
    {
      input.startInputUnit(InputUnit.CHAR);
      input.setGuideBox(DIGITAL_INK[i][0][0],
                        DIGITAL_INK[i][0][1],
                        DIGITAL_INK[i][0][2],
                        DIGITAL_INK[i][0][3]);
      
      
      // for each stroke
      for (int j = 1; j < DIGITAL_INK[i].length; ++j)
      {
        input.addStroke(DIGITAL_INK[i][j], 1, 2,            // x coordinates
                        DIGITAL_INK[i][j], 2, 2,            // y coordinates
                        (DIGITAL_INK[i][j].length - 1) / 2, // point count
                        (int)DIGITAL_INK[i][j][0]);         // stroke index
        
        // The stroke index is important. If you don't pass it to the tagger,
        // then the "8" in the ink will not be considered as being erased.
      }
    }
    
    System.out.println(" . input fueled successfully");
    System.out.println();
    
    System.out.println("6) setting the input as the source of the tagger");
    tagger.setSource(input);
    System.out.println(" . input successfully set as the tagger's source");
    System.out.println();
    
    System.out.println("7) running the digital ink tagging process");
    tagger.run();
    System.out.println(" . digital ink tagging performed successfully");
    System.out.println();
    
    System.out.println("8) retrieving the TagList");
    TagList tags = tagger.getTags();
    System.out.println(" . TagList retrieved successfully");
    System.out.println();
    
    System.out.println("9) iterating over the tags in the TagList");
    final TagIterator iterator = tags.getIterator();
    
    while(!iterator.isAtEnd())
    {
      System.out.println( " . " + iterator.getType().getName() +
                          ", " + iterator.getInputRange());

      iterator.next();
    }
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
    iterator.dispose();
    tags.dispose();
    input.dispose();
    tagger.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform digital ink tagging");
  }

  /**
   * We did want to emphasize the usage of MyScript SDK, not how to load ink
   * from a file. This is why we hard coded the stroke data.
   * 
   * You can vizualize this ink by opening the
   * edk/examples/ink/DigitalInkTagging.itf file.
   * 
   * <ul>
   * <li><code>DIGITAL_INK.length</code> gives you the number of boxes</li>
   * <li><code>DIGITAL_INK[0]</code> gives you access to the first box</li>
   * <li><code>DIGITAL_INK[0].length - 1</code> gives you the number of strokes
   * in the first box</li>
   * <li><code>DIGITAL_INK[0][Ø]</code> gives you the guide box of the first
   * box</li>
   * <li><code>DIGITAL_INK[0][1]</code> gives you the first stroke of the first
   * box</li>
   * <li><code>DIGITAL_INK[0][2]</code> gives you the second stroke of the first
   * box</li>
   * </ul>Stroke coordinates are interleaved: <code>x1, y1, x2, y2,</code> etc. 
   */
  private static float[][][] DIGITAL_INK =
  {
    // box 0: "2"
    {
      // guide box
      {
        500, 200, 50, 70
      },
      
      // stroke 0
      {
        // global stroke index in the ink
        0,
        
        // x, y
        519, 229,
        516, 229,
        515, 229,
        515, 227,
        514, 226,
        514, 224,
        516, 223,
        518, 221,
        520, 219,
        522, 218,
        525, 217,
        527, 217,
        529, 217,
        530, 217,
        530, 219,
        530, 222,
        530, 225,
        529, 228,
        528, 232,
        525, 236,
        522, 239,
        519, 243,
        516, 246,
        513, 249,
        510, 251,
        508, 254,
        505, 256,
        504, 258,
        504, 259,
        504, 260,
        505, 261,
        508, 261,
        512, 261,
        516, 261,
        521, 261,
        526, 260,
        530, 259,
        534, 259
      }
    },

    // box 1: empty
    {
      // guide box
      {
        550, 200, 50, 70
      }
      
      // the first "0" started in this box but it was assigned to the next box
      // because of the missing pen lift between the two "0" digits in the
      // digital ink.
    },

    // box 2: "00"
    {
      // guide box
      {
        600, 200, 50, 70
      },
      
      // stroke 0
      {
        // global stroke index in the ink
        1,
        
        // x, y
        586, 229,
        586, 227,
        584, 226,
        583, 225,
        581, 225,
        578, 225,
        576, 226,
        573, 228,
        570, 231,
        568, 235,
        566, 239,
        566, 244,
        566, 248,
        567, 252,
        570, 255,
        573, 257,
        577, 257,
        582, 256,
        586, 253,
        590, 249,
        593, 244,
        596, 239,
        597, 235,
        597, 231,
        597, 228,
        595, 226,
        593, 226,
        591, 226,
        590, 226,
        588, 228,
        587, 229,
        587, 231,
        587, 232,
        589, 234,
        592, 235,
        595, 236,
        599, 236,
        604, 237,
        610, 237,
        616, 236,
        622, 235,
        627, 233,
        632, 232,
        636, 230,
        640, 228,
        643, 226,
        645, 224,
        646, 222,
        646, 221,
        646, 219,
        645, 218,
        643, 218,
        640, 218,
        637, 219,
        633, 221,
        628, 223,
        624, 227,
        620, 231,
        616, 236,
        613, 240,
        611, 245,
        610, 249,
        610, 253,
        611, 256,
        614, 258,
        618, 258,
        622, 258,
        626, 256,
        631, 252,
        635, 248,
        639, 243,
        641, 238,
        642, 233,
        642, 229,
        640, 226,
        638, 223,
        635, 222,
        631, 222
      }
    },
    
    // box 3: "7" and a stroke meant to scratch out both the "7" and the "8"
    {
      // guide box
      {
        650, 200, 50, 70
      },
      
      // stroke 0
      {
        // global stroke index in the ink
        2,
        
        // x, y
        664, 214,
        663, 214,
        663, 215,
        664, 216,
        665, 217,
        668, 218,
        670, 219,
        674, 220,
        677, 220,
        680, 220,
        683, 220,
        686, 220,
        688, 220,
        689, 219,
        689, 221,
        687, 223,
        685, 226,
        682, 229,
        680, 233,
        677, 238,
        675, 242,
        673, 246,
        672, 249,
        671, 252,
        671, 254,
        670, 255,
        670, 254,
        670, 253
      },
      
      // stroke 1
      {
        // global stroke index in the ink
        3,
        
        // x, y
        656, 240,
        657, 240,
        658, 241,
        661, 241,
        664, 241,
        668, 241,
        672, 241,
        676, 241,
        681, 240,
        685, 240,
        688, 239,
        691, 239
      },
      
      // stroke 2 (stroke meant to scratch out the "7" and the "8")
      {
        // global stroke index in the ink
        5,
        
        // x, y
        646.25f,  217.608f,
        653.977f, 217.608f,
        661.703f, 217.608f,
        669.43f,  215.088f,
        677.157f, 213.828f,
        687.459f, 212.569f,
        700.337f, 211.309f,
        710.64f,  208.789f,
        720.942f, 207.529f,
        731.244f, 206.27f,
        738.971f, 205.01f,
        746.698f, 205.01f,
        751.849f, 203.75f,
        754.424f, 203.75f,
        751.849f, 205.01f,
        746.698f, 206.27f,
        741.547f, 208.789f,
        733.82f,  211.309f,
        723.517f, 213.828f,
        710.64f,  217.608f,
        700.337f, 221.387f,
        690.035f, 223.907f,
        679.733f, 226.426f,
        672.006f, 230.206f,
        664.279f, 231.466f,
        661.703f, 232.725f,
        661.703f, 233.985f,
        669.43f,  233.985f,
        677.157f, 232.725f,
        687.459f, 231.466f,
        697.762f, 230.206f,
        710.64f,  227.686f,
        723.517f, 225.167f,
        733.82f,  222.647f,
        744.122f, 221.387f,
        751.849f, 220.127f,
        757,      218.868f,
        749.273f, 218.868f,
        741.547f, 221.387f,
        731.244f, 222.647f,
        720.942f, 225.167f,
        708.064f, 227.686f,
        692.61f,  230.206f,
        679.733f, 233.985f,
        669.43f,  236.505f,
        659.128f, 239.025f,
        651.401f, 241.544f,
        646.25f,  244.064f,
        648.826f, 244.064f,
        656.552f, 244.064f,
        664.279f, 242.804f,
        677.157f, 241.544f,
        687.459f, 239.025f,
        700.337f, 237.765f,
        710.64f,  235.245f,
        718.366f, 233.985f,
        726.093f, 232.725f,
        728.669f, 232.725f,
        728.669f, 231.466f,
        723.517f, 232.725f,
        715.791f, 233.985f,
        708.064f, 235.245f,
        697.762f, 237.765f,
        687.459f, 241.544f,
        677.157f, 244.064f,
        666.855f, 247.843f,
        659.128f, 250.363f,
        651.401f, 252.882f,
        648.826f, 255.402f,
        648.826f, 256.662f,
        656.552f, 256.662f,
        664.279f, 256.662f,
        672.006f, 255.402f,
        682.308f, 254.142f,
        695.186f, 251.623f,
        702.913f, 249.103f,
        710.64f,  247.843f,
        718.366f, 246.583f,
        723.517f, 245.324f,
        728.669f, 244.064f,
        731.244f, 244.064f,
        726.093f, 246.583f,
        720.942f, 247.843f,
        715.791f, 250.363f,
        708.064f, 252.882f,
        700.337f, 255.402f,
        692.61f,  257.922f,
        684.884f, 261.701f,
        677.157f, 264.221f,
        674.581f, 265.48f,
        674.581f, 266.74f,
        674.581f, 268,
        682.308f, 268,
        690.035f, 268,
        700.337f, 266.74f
      }
    },
    
    // box 4: "8"
    {
      // guide box
      {
        700, 200, 50, 70
      },
      
      // stroke 0
      {
        // global stroke index in the ink
        4,
        
        // x, y
        719, 236,
        718, 236,
        718, 237,
        719, 237,
        720, 235,
        722, 234,
        723, 232,
        725, 230,
        726, 228,
        726, 225,
        726, 223,
        726, 221,
        725, 219,
        723, 218,
        722, 217,
        720, 217,
        719, 217,
        718, 217,
        718, 218,
        717, 220,
        717, 221,
        718, 224,
        719, 226,
        720, 228,
        722, 231,
        723, 234,
        725, 237,
        727, 240,
        728, 243,
        729, 245,
        730, 247,
        730, 249,
        730, 251,
        730, 252,
        729, 253,
        728, 254,
        726, 254,
        723, 255,
        720, 255,
        718, 254,
        715, 253,
        712, 252,
        710, 251,
        708, 249,
        707, 246,
        707, 244,
        707, 242,
        708, 240,
        710, 238,
        712, 236,
        715, 235,
        719, 234    
      }
    },
    
    // box 5: "9" - emphasized
    {
      // guide box
      {
        750, 200, 50, 70
      },
      
      // stroke 0
      {
        // global stroke index in the ink
        6,
        
        // x, y
        781,      216.25f,
        780.75f,  216f,
        780.5f,   215.75f,
        780.25f,  215.5f,
        778.75f,  215,
        778.25f,  214.75f,
        776.75f,  214.25f,
        776.25f,  214.25f,
        775.5f,   214.25f,
        775,      214.25f,
        774,      215.25f,
        773.75f,  215.75f,
        771.25f,  216.75f,
        770.25f,  218.25f,
        769.75f,  220.25f,
        769.75f,  222.25f,
        769.75f,  224.25f,
        769.75f,  226.25f,
        770.75f,  228.25f,
        771.75f,  229.75f,
        773.25f,  231.25f,
        774.75f,  231.75f,
        775.5f,   231.75f,
        777,      232.25f,
        779,      231.25f,
        780.5f,   230.75f,
        781.5f,   229.75f,
        782.5f,   228.75f,
        783,      227.25f,
        783.5f,   225.75f,
        783.5f,   225,
        783.5f,   223,
        783.5f,   221,
        783,      219.5f,
        783,      219,
        783,      218.25f,
        783,      217.75f,
        783,      217.25f,
        783,      217,
        783,      216.75f,
        783,      217,
        783,      217.5f,
        783,      218.25f,
        783,      219,
        783.5f,   221.5f,
        784,      224f,
        784.5f,   227,
        785,      230.5f,
        785.5f,   234,
        786,      237,
        786,      240,
        786,      243,
        785.5f,   245.5f,
        785,      247,
        784.5f,   248.5f,
        783,      250.5f,
        782,      252,
        780,      252.5f,
        778,      253,
        775.5f,   252.5f,
        773,      252.5f,
        770.5f,   251.5f,
        768.5f,   251,
        768,      250.75f,
        768,      250.5f
      },
      
      // stroke 1
      {
        // global stroke index in the ink
        7,

        // x, y
        781.5f,   219.5f,
        781.5f,   219,
        781,      218.5f,
        781,      218,
        780,      217.5f,
        779.5f,   217,
        778.5f,   216.5f,
        778,      216,
        777,      216,
        776,      216.5f,
        775.5f,   217,
        774.5f,   217.5f,
        772.5f,   219.5f,
        772,      220,
        771,      223,
        771,      224.5f,
        771,      225.5f,
        770,      228.5f,
        770,      230,
        772,      232,
        772.5f,   233,
        773.5f,   233.5f,
        774,      233.5f,
        775,      233.5f,
        776,      233.5f,
        777,      233,
        777.5f,   232.5f,
        779.5f,   230.5f,
        780,      229.5f,
        780.5f,   228.5f,
        781.5f,   225.5f,
        782,      224.5f,
        783,      221.5f,
        783,      220.5f,
        783,      219.5f,
        783,      219,
        783,      218.5f,
        783.5f,   218.5f,
        783.5f,   219,
        783.5f,   219.5f,
        783.5f,   220,
        784,      221,
        784,      222.5f,
        785,      226.5f,
        786,      230.5f,
        786,      235.5f,
        787,      239.5f,
        787,      244.5f,
        788,      249.5f,
        788,      251,
        788,      252.5f,
        787.5f,   253,
        787,      253.5f,
        786,      254,
        785,      254,
        783.5f,   254,
        782,      254,
        780.5f,   254,
        776.5f,   254,
        772.5f,   254,
        771.5f,   254,
        771.5f,   255
      }
    }
  };  
  
} // DigitalInkTagging
