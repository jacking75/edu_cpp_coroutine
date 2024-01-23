# C++ 코루틴 학습
  
# 추천 학습 방법 
- 코루틴에 대한 설명은 아래 글이 아주 아주 좋다. 이 글을 여러번 보면서 꼭 이해하는 것을 추천한다.
    - https://kukuta.tistory.com/222
- 박동하님의 강의 영상을 보시면 좋다. 단 이 강의는 기초를 설명하는 것은 아니라서 위 글을 보고 난 후 보는 것이 좋다
    - https://youtu.be/F6E_cLOIe-U
- 코루틴 라이브러리를 개발 
    - 박동하님이 깃허브에 올린 글과 코드를 참고하면 좋다.
        - https://github.com/luncliff/coroutine
        - 코루틴 설명은 여기에  https://github.com/luncliff/coroutine/tree/master/docs
        - [Russian Roulette and C++ Coroutines](https://github.com/luncliff/coroutine/blob/main/docs/articles/russian-roulette-kor.md )
        - 코루틴 라이브러리 분석은 아래 저장소에 있는 테스트 코드를 시작으로 관련 코드의 흐름을 하나씩 따라가보면 라이브러리로 어떻게 만드는지 배우는데 좋습니다.  https://github.com/luncliff/coroutine/tree/master/test

# 학습 자료
- [CppKoreaSeminar5th/5 - C++ Coroutine 알아보기; 접근법, 컴파일러, 그리고 이슈](https://github.com/CppKorea/CppKoreaSeminar5th/tree/master/5%20-%20C%2B%2B%20Coroutine%20%EC%95%8C%EC%95%84%EB%B3%B4%EA%B8%B0%3B%20%EC%A0%91%EA%B7%BC%EB%B2%95%2C%20%EC%BB%B4%ED%8C%8C%EC%9D%BC%EB%9F%AC%2C%20%EA%B7%B8%EB%A6%AC%EA%B3%A0%20%EC%9D%B4%EC%8A%88 )
- C++ Korea 제6회 세미나 - System API와 Coroutine 결합하기 (박동하) [유튜브](https://youtu.be/F6E_cLOIe-U )  [문서](https://github.com/CppKorea/CppKoreaSeminar5th)
- [Slides for Meeting C++ 2019: C++20 Coroutines](https://meetingcpp.com/mcpp/slides/2019/C20%20Coroutines.pdf )
- [A map through the three major coroutine series](https://devblogs.microsoft.com/oldnewthing/?p=105178 )
- [C++ links: Coroutines](https://gist.github.com/MattPD/9b55db49537a90545a90447392ad3aeb )
- [(일어) C++에서 async/await 하기](https://qiita.com/tan-y/items/6033ab9e7298999bf214)
- [(일어) C++20 코루틴으로 놀기 with OpenSiv3D](https://qiita.com/tyanmahou/items/1799d80c9e260b2267d5  )
       
    
<br>  
     
# 라이브러리 분석

## tinycoro  
- https://github.com/asmei1/tinycoro 
- 코루틴을 epoll에 사용하는 방법을 알아볼 때 유용  
  

  
## coro: Single-header library facilities for C++2a Coroutines
- https://github.com/Quuxplusone/coro
- https://quuxplusone.github.io/blog/2019/07/03/announcing-coro-examples/
  
  
## cppcoro  
- [GitHub](https://github.com/lewissbaker/cppcoro )
- [(일어) C++20 코루틴 for 애플리케이션 cppcoro](https://qiita.com/Fuyutsubaki/items/a4c9921587ce53d95e55 )
- [(일어) C++ 비동기 처리의 현재 및 미래를 위한 코루틴 cppcoro](https://qiita.com/Fuyutsubaki/items/d02a68d42a022661cfd6 )
- [A C++20 coroutine example](https://mariusbancila.ro/blog/2020/06/22/a-cpp20-coroutine-example/ )
- [C++20: Coroutines with cppcoro](http://www.modernescpp.com/index.php/c-20-coroutines-with-cppcoro )
- [C++20: Powerful Coroutines with cppcoro](http://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro )
- [C++20: Thread Pools with cppcoro](http://www.modernescpp.com/index.php/c-20-thread-pools-with-cppcoro )


    
## Coop   
- Coop is a C++20 coroutines-based library to support cooperative multitasking in the context of a multithreaded application
- https://github.com/jeremyong/coop


  
##  kuro 
- https://github.com/X-Neon/kuro 
- A C++20 coroutine library, somewhat modelled on Python's asyncio
    

   
## libcopp
- https://github.com/owent/libcopp

  
## Coro-Async : A lite C++ coroutine networking library | Introduction
- http://templated-thoughts.blogspot.com/2018/11/coro-async-lite-c-coroutine-networking.html

    
## edubart/minicoro
- https://github.com/edubart/minicoro 
  
  
