using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.InputSystem;

public class Cube : MonoBehaviour
{ 
    bool b_pressed = false;
    bool ok_pressed = false;

    public Color cubeColor;
    public Mode md;
    public GameObject menuCanvas;
    
    // Start is called before the first frame update
    void Start()
    {
        GetComponent<Outline>().enabled = false;
        cubeColor = GetComponent<Renderer>().material.color;
    }

    // Update is called once per frame
    void Update()
    {
        b_pressed = Input.GetButtonDown("B");
        ok_pressed = Input.GetButtonDown("OK");
        if(GetComponent<Outline>().enabled && !menuCanvas.activeInHierarchy)
        {
            if(Input.GetButton("B"))
            {
                switch (md.getMode().mode)
                {
                    case "Move":
                        transMode();
                        break;
                    case "Rotate":
                        rotateMode();
                        break;
                    case "Color":
                        if(b_pressed)
                            colorMode();
                        break;
                    default:
                        break;
                }
            }
            if(ok_pressed)
            {
                EnableMenu();
            }
        }
    }

    public void transMode()
    {
        Vector3 move = new Vector3();
        int moveValue = md.getMode().moveValue;
        switch (md.getMode().translationAxis)
        {
            case "X-Axis":
                move = new Vector3(moveValue, 0, 0) * Time.deltaTime;
                break;
            case "Y-Axis":
                move = new Vector3(0, moveValue, 0) * Time.deltaTime;
                break;
            case "Z-Axis":
                move = new Vector3(0, 0, moveValue) * Time.deltaTime;
                break;
            default:
                break;
        }
        transform.Translate(move, Space.World);
    }

    public void rotateMode()
    {
        Vector3 rotate = new Vector3();
        int rotateValue = md.getMode().rotateValue;
        switch (md.getMode().rotationAxis)
        {
            case "X-Axis":
                rotate = new Vector3(rotateValue, 0, 0) * 1f * Time.deltaTime;
                break;
            case "Y-Axis":
                rotate = new Vector3(0, rotateValue, 0) * 1f * Time.deltaTime;
                break;
            case "Z-Axis":
                rotate = new Vector3(0, 0, rotateValue) * 1f * Time.deltaTime;
                break;
            default:
                break;
        }
        transform.Rotate(rotate, Space.World);
    }

    public void colorMode()
    {
        switch (md.getMode().colorMode)
        {
            case "Fixed":
                if(GetComponent<Renderer>().material.color == cubeColor)
                {
                    GetComponent<Renderer>().material.color = Color.red;
                }
                else
                {
                    GetComponent<Renderer>().material.color = cubeColor;
                }
                break;
            case "Random":
                GetComponent<Renderer>().material.color = Random.ColorHSV(0f, 1f, 1f, 1f, 0.5f, 1f);
                break;
            default:
                break;
        }
    }

    public void EnableMenu()
    {
        gameObject.transform.FindChild("Canvas").gameObject.SetActive(true);
    }
    
    // Enable outline if pointer touches cube
    public void PointerEnter()
    {
        if(!string.IsNullOrEmpty(md.getMode().mode))
        {
            GetComponent<Outline>().OutlineColor = Color.green;
        } 
        else
        {
            GetComponent<Outline>().OutlineColor = Color.black;
        }

        GetComponent<Outline>().enabled = true;
    }

    // Disable outline if pointer does not touch cube
    public void PointerExit()
    {
        GetComponent<Outline>().enabled = false;
    }
}
