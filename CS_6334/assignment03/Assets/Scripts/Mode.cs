using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Linq;

public class Mode : MonoBehaviour
{
    // Button Values
    public struct ModeType
    {
        public string mode;
        public int moveValue;
        public string translationAxis;
        public int rotateValue;
        public string rotationAxis;
        public string colorMode;
    }
    public static ModeType m;
    public GameObject moveValueText, rotateValueText;
    public ToggleGroup transToggleGroup, rotateToggleGroup, colorToggleGroup;

    Quaternion spawnRot = Quaternion.Euler(0f, 0f, 0f);

    // Start is called before the first frame update
    void Start()
    {
        // Struct Initilization
        m.mode = "";
        m.moveValue = 5;
        m.translationAxis = "Z-Axis";
        m.rotateValue = 30;
        m.rotationAxis = "Z-Axis";
        m.colorMode = "Random";
    }

    // Update is called once per frame
    void Update()
    {
        moveValueText.GetComponent<Text>().text = m.moveValue.ToString();
        rotateValueText.GetComponent<Text>().text = m.rotateValue.ToString();
    }

    void LateUpdate()
    {
        transform.rotation = spawnRot;
    }

    public ModeType getMode()
    {
        return m;
    }

    public void incrememtMoveValue()
    {
        m.moveValue += 2;
    }

    public void decrementMoveValue()
    {
        m.moveValue -= 2;
    }

    public Toggle currentTransSelection
    {
        get { return transToggleGroup.ActiveToggles().FirstOrDefault(); }
    }

    public void updateTransAxis()
    {
        m.translationAxis = currentTransSelection.name;
    }

    public void incrementRotateValue()
    {
        m.rotateValue += 15;
    }

    public void decrementRotateValue()
    {
        m.rotateValue -= 15;
    }

    public Toggle currentRotateSelection
    {
        get { return rotateToggleGroup.ActiveToggles().FirstOrDefault(); }
    }

    public void updateRotateAxis()
    {
        m.rotationAxis = currentRotateSelection.name;
    }

    public Toggle currentColorSelection
    {
        get { return colorToggleGroup.ActiveToggles().FirstOrDefault(); }
    }

    public void updateColorMode()
    {
        m.colorMode = currentColorSelection.name;
    }

    public void setMoveMode()
    {
        m.mode = "Move";
    }

    public void setRotateMode()
    {
        m.mode = "Rotate";
    }

    public void setColorMode()
    {
        m.mode = "Color";
    }

    public void clearMode()
    {
        m.mode = "";
    }
}