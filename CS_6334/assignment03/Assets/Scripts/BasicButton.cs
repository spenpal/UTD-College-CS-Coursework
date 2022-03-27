using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

public class BasicButton : MonoBehaviour
{
    bool b_pressed = false;
    bool selected = false;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        b_pressed = Input.GetButtonUp("B");
        if(selected && b_pressed)
            GetComponent<Button>().onClick.Invoke();
    }

    public void Select()
    {
        selected = true;
    }
    
    public void UnSelect()
    {
        selected = false;
    }

    public void Reset()
    {
        b_pressed = false;
        selected = false;
    }
}