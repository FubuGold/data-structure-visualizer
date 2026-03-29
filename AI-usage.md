# AI Usage
- ChatGPT: 
    - GPT-5.2, OpenAI, chatgpt.com, accessed 13:00 on May 25, 2025, prompt about the commands in Makefile; AI generate sample code and explaination, student revised and validated with https://makefiletutorial.com/ and tested in practise. Note that the conversation has been lost and the proof is only in the commit history. 

    - GPT-5.2, OpenAI, chatgpt.com, prompt about the solution to call class with template with pointer; AI generate a solution with sample code and explaination, student revised and checked with https://stackoverflow.com/questions/63961106/creating-a-container-to-store-template-class-instantiations and tested in practise. Note that the time of the chat was not recorded and lost due to accidentally missed clicked. Proof is in `AI-proof/1`. Prompt: 
    ```
    template<typename callback_t, typename ...Args> 
    requires std::invocable<callback_t, Args...> 
    class InteractiveElement : public BaseElement 
    std::vector<std::shared_ptr<InteractiveElement>> elements; 
    i want the vector to store interative element for any template
    ```

    - March 26, 9:00 P.M., GPT-5.2, OpenAI, chatgpt.com, prompt about the solution for removing the different between Figma and SFML text; AI generate a solution with sample code and explaination, student revised and checked with visual cue and did some small tweak to the numbers. Proof is in `AI-proof/2`. Prompt:
    ```
    the text in figma and the text in sfml render in slight different position even though i have set the origin to position of local bound. how to fix?
    ```

    - March 26, 10:30 P.M., GPT-5.2, OpenAI, chatgpt.com, prompt to fix missing thumbnail for AVL with a follow up to give more context; AI generate a solution with sample code and explaination, student did not use the solution as the bug in not in context, and it was resolved manually without help. Proof is in `AI-proof/3`. Prompts:
    ```
    sf::Texture thumbnailImg[6]; static constexpr std::string filename[6] = { "SLL.png","Heap.png","AVL.png","Trie.png","SP.png","MST.png" }; 
    ////////// 
    for (int i=0;i<6;i++) { assert(thumbnailImg[i].loadFromFile("./asset/thumbnail/" + filename[i])); } 
    ///////
    std::shared_ptr<sf::Sprite> avlThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[AVL]); avlThumbnail->setPosition({66,203}); addElement(avlThumbnail); thumbnail is white while the other still display
    ```
    ```
    it is a member in class
    ```

