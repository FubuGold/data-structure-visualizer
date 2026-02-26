# AI Usage
- ChatGPT: 
    - GPT-5.2, OpenAI, chatgpt.com, accessed 13:00 on May 25, 2025, prompt about the commands in Makefile; AI generate sample code and explaination, student revised and validated with https://makefiletutorial.com/ and tested in practise. Note that the conversation has been lost and the proof is only in the commit history. 

    - GPT-5.2, OpenAI, chatgpt.com, prompt about the solution to call class with template with pointer; AI generate a solution with sample code and explaination, student revised and checked with https://stackoverflow.com/questions/63961106/creating-a-container-to-store-template-class-instantiations and tested in practise. Note that the time of the chat was not recorded and lost due to accidentally missed clicked. Proof is in AI-proof/2. Prompt: \
    template<typename callback_t, typename ...Args> \
    requires std::invocable<callback_t, Args...> \
    class InteractiveElement : public BaseElement \
    std::vector<std::shared_ptr<InteractiveElement>> elements; \
    i want the vector to store interative element for any template
