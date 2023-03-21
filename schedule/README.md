# Friday
## 9.00am - Opening Ceremony - Kylie & Silvo

## 9.10am - Keynote - "Quantum is Coming…" - Elanor Huntington
Three constellations of technologies are routinely cited these days as being the instruments through which geopolitical strategies and massive economic and societal disruption will be enacted in the coming decades: artificial intelligence; space engineering; and quantum computing. There’s no doubt that two of those three technologies have already been profoundly disruptive, ranging all the way from working out where to buy coffee in an unfamiliar city through to influence on Presidential elections in previously stable democracies. The third, quantum computing, is moving out of the furthest reaches of weird and quirky and increasingly being viewed as a technology of strategic importance. 

In this presentation, I will chart the trajectory of the development of quantum technologies and continue the throughline into where these may go over the next decades. I then go on to talk about some of the interesting possibilities that arise from the interplay between those three technologies as well as the strategic, economic and cultural ambitions of the society in which they’re embedded.

### Bio:
I'm the first female Dean of Engineering and Computer Science at the Australian National University and still one of the few in the world. I'm committed to growing the profile of Engineering in the community and am passionate about attracting more young women to take up careers that draw on STEM skills (@profelanor). 
In June 2014, I was appointed the Dean of the ANU College of Engineering and Computer Science and prior to that was the Head of the School of Engineering and Information Technology with UNSW Canberra at the Australian Defence Force Academy. 
I am excited to be leading a project to reimagine a new type of engineering and computing, fit for the middle of the 21st century, which is all about understanding and shaping large-scale systems of people acting and interacting with each other through their digital and physical environments. 
I have a PhD (2000) in experimental quantum optics from the ANU and my current research interests are in the control of quantum systems, with a particular interest in the interface between theory and applications. I am also a program manager in the ARC Centre of Excellence for Quantum Computation and Communication Technology. 
I'm an enthusiastic but not particularly effective soccer player. I can usually be found on Sundays somewhere in the frozen wastelands of a Canberra soccer pitch, playing the old ladies league. 

## 10.00am - Break

## 10.15am - "Abuse of trust - Exploiting our relationship with public repositories" - PatH
Developers and admin around the world use and trust public repositories, such as Python's PyPi, to provide secure means to download, install, and manage 3rd part libraries and tools. But is this trust misplaced? 

This talk will cover how PyPi can be abused to leak personal information, and provide malware C2 and exfiltration channels. This talk will also cover how to help the open source community and ourselves improve the balance between usability and security. 

### Bio:
Pat (@path_to_file) is a software developer who in his spare time likes inverting security assumptions, his dog Cluedo, and baseball. His lifelong goal is to combine all three to create the world’s first Sneakers-AirBud mashup video. 

## 10.45am - "Coring Apple WiFi" - Steve Glass
iPhones have an enviable reputation for security. The close link between hardware and software, curated AppStore, and frequent security updates make exploitation of the iPhone difficult. This has, however, not deterred malware companies and others from compromising iOS devices exploit security flaws in the browser, the 4G baseband processor, and the Wireless Network Interface Controller (WNIC). 

In this presentation we focus on exploiting the WNIC. This avoids many of the defences built into iOS and exposes a new set of otherwise inaccessible attack surfaces within the iOS kernel. We’ll also take a look at how Apple makes the iOS WiFi stack “think different”. This includes a brief coverage of Bonjour and the Apple Wireless Direct Link (AWDL) protocol that’s at the heart of AirDrop, AirPlay, and similar services. These protocols have been implemented, in part, by offloading some of their processing onto the WNIC itself - and we’ll look at how this might help exploit the device. The approach taken here follows in the steps of Gal Beniamini’s Project Zero articles and that of SEEMOO's NexMon framework and we’ll bring things up-to-date in light of Apple’s latest security controls. In practical terms, we’ll show how to find, analyse, reverse engineer, and patch the WNIC's firmware and conclude with a discussion of WiFi fuzzing and exploit hunting. 

### Bio:
Steve is a developer, researcher, and educator specialising in network and software security 

## 11.45am - "Deobfuscating JavaScript malware" - Adrian Herrera
JavaScript code can be hard enough to read and understand, even when it’s been well engineered. Now imagine that you’re a malware analyst who needs to understand some malicious JavaScript that has been purposefully obfuscated; what do you do? One approach is to add instrumentation and do a dynamic analysis, but in doing so we may miss important details. Another approach is to perform a static analysis and try to undo the obfuscation. This talk is about the later, and explores how we can borrow techniques from compiler theory to build a deobfuscator for malicious JavaScript. 

### Bio:
Adrian (@0xadr1an) is a research engineer with the Australian Defence Science and Technology (DST) Group and a PhD student at the Australian National University (ANU). His interests are in reverse engineering, program analysis and software security. In particular, how to apply program analysis techniques from academia to solve practical reverse engineering problems. 

## 12.15pm - Lunch

## 1.15pm - "Kubernetes Security" - Ben Cambourne
Kubernetes is the leading container orchestration system, many organisations have deployed it, or are starting to deploy it. But, how secure is it? This talk will cover the security of Kubernetes, common mistakes made, and how to secure your cluster. 

### Bio:
Ben is a Security Researcher at elttam. He has over a decade of professional experience in both offensive and defensive roles. 

He enjoys (over-)engineering elegant solutions, solving challenges, honing his craft and sharing with others. When not getting a tan from his 7 screen Swordfish setup he likes adventuring outdoors. 

## 2.15pm - "JWAT.... Attacking JSON Web Tokens" - Louis
Nowadays, JSON Web Tokens are everywhere. They are used as session tokens or just to pass data between applications or µservices. By design, JWT contains a high number of security and cryptography pitfalls. In this talk, we are going to learn how to exploit (with demos) some of those issues. 

### Bio:
Louis (@pentesterlab)is a security engineer based in Melbourne, Australia. He performs pentest, architecture and code review. Louis is the founder of PentesterLab, a learning platform for web penetration testing. 

## 3.15pm - "Does Formal Verification Really Lead to Better Security?" - Ihor Kuz
The hype is that formally verification and mathematical proof give you hack-proof systems. But of course that's not really true. So why is there increasingly more interest around formal verification and security? 

In this talk I'll look at what formal verification does give you and how this can be leveraged to improve security and safety of systems. I'll also talk about what it doesn't give you and what other measures are necessary for security. This will be based on work we've done on seL4 and related technologies and experience in building systems using these. 

### Bio:
Ihor (@m3lcee) has spent a lifetime playing with and programming computers and is now a principal research engineer at Data61/CSIRO working on microkernel-based operating systems and high-assurance software. He is part of the team responsible for seL4 - the formally verified operating systems kernel. He is also a senior lecturer at UNSW, teaching distributed systems and supervising amazing students. 

When not at the computer Ihor likes to spend time outdoors doing activities that could lead to bruises and broken bones, but is smart enough to (usually) avoid those results. 

## 3.45pm - Break

## 4.00pm - "Binary Patching - Making the Switch" - Pete Rankin
Despite its open source roots, the Android security ecosystem is still a black box for most users. To make his Nintendo Switch controllers work with his Android phone, Peter undertook binary patch analysis to uncover and develop a patch for the Android Bluetooth process. This research also identified pathways to patch other features of the phone, specifically the TrustZone kernel driver using an Inline hook to patch and enable interception of secure world communications between user mode processes and Trustlets. 

Due to significant improvements to the Android security model and chain of trust, this task was unexpectedly difficult, and required rebuilding of the phone's kernel, and implementation of a kernel-to-user in-memory patching driver to avoid unintended changes to the Android system. 

This talk will walk through the techniques and tools used to peer into the Android security black box, and come up with creative solutions to tricky problems, including patching kernel code, making in-memory changes to a running application and building/packaging an Android kernel. 

### Bio:
Peter (@rankstar591) is a secure software developer for Penten, solving hard problems in the mobile space. Outside of work he enjoys making devices do things they shouldn’t and then never using them. Peter has previously worked as a software engineer for Australian Department of Defence, Robert Bosch and Thales Australia. 

## 5.00pm - "The fresh smell of ransom coffee" - Martin Hron
Let’s have a look at how breaking best security practices in smart devices can potentially lead to serious consequences. This talk presents a case of a coffee maker where replacing a firmware can be done remotely without the user’s interaction. The point here is that these devices could be easily turned into a hidden spy, slowly exfiltrating data out of your perimeter or as a tool of extortion. However this talk is on a funny note, the message is clear if we won’t address security in IoT than we are facing a big problem in the future. Come and join me on this journey where S stands for security in IoT. 

### Bio:
Currently security researcher at Avast. I lead research across various disciplines such as dynamic binary translation, hardware-assisted virtualization, IoT, firmware vulnerabilities and malware analysis. I'm devoted to technology and I'm a true software and hardware reverse engineer, game programmer, tinkerer, AI and IoT mantras practitioner with deep knowledge of OS, CPU and HW architectures. Prior to my current job I've worked as artificial intelligence and game programmer, working on the MAFIA II game project and windows kernel SW engineer with encryption file system drivers. I've got almost 25 years experience in this domain (@thinkcz). 

## 5.30pm - Day 1 close

## 6.30pm - Please join us for the Friday Night Party!!

# Saturday
## 9.00am - Welcome to Day 2

## 9.10am - Keynote - "What's in a Jailbreak? Hacking the iPhone: 2014 -> 2019" - Mark Dowd
The iPhone is one of the most secure commercially available devices today. Apple has aggressively pursued hardware and software solutions to pre and post-exploitation security weaknesses to prevent all but the most determined attacker's from getting complete access to the device and the personal data it contains. This talk will be a tour of the last 5 years worth of major security enhancements - both in software and in hardware, and talk about the evolving complexity involved in developing a full-featured "jailbreak". The talk will consider the impact of various mitigations on both remote access (browser or no-click) and local access (USB-based) vectors, as well as discussing post-exploitation challenges, which in some cases are just as formidable as exploitation itself.

### Bio
Mark Dowd (@mdowd) is an expert in application security, specializing primarily in low level operating system vulnerability research for both desktops and popular mobile devices.. He is currently the director of Azimuth Security (now L3 Trenchant), a security company he founded that specializes in code review and cutting edge security research. Prior to starting Azimuth, his professional experience includes several years as a senior researcher at a fortune 500 company, where he uncovered a variety of major vulnerabilities in ubiquitous Internet software. He also worked as a Principal Security Architect for McAfee, where he was responsible for internal code audits, secure programming classes, and undertaking new security initiatives. Mark has also co-authored a book on the subject of application security named "The Art of Software Security Assessment," and has spoken at several industry-recognized conferences. 

## 10.00am - Break

## 10.15am - "SSH, so hot right now. Profiling it with HASSH" - Ben Reardon & Adel Karimi
Is that SSH client/server really what it says it is ? Now you can tell this and more - with HASSH! Looking for signals in the initialization of encrypted communication channels is not a new concept. There are many examples of fingerprinting both unencrypted and encrypted protocols such as TLS. However somewhat surprisingly, no open source scalable fingerprinting method has been developed for one of our most common and relied upon encrypted protocols SSH — an integral component of the internet. Enter, the HASSH. 

HASSH is a network fingerprinting standard. It can be used to help identify specific Client and Server SSH implementations. These fingerprints can be easily stored, searched and shared in the form of a standard string of summary text, a hassh for the Client and hasshServer for the Server. Gaining a greater insight into the observable nature of SSH clients and servers opens up a few really interesting possibilities. HASSH can highlight Deceptive implementations, Detect novel exfiltration attempts within the SSH negotiation packets themselves, baseline devices including IOT devices, make a passive assessment of patch levels of SSH servers and clients, and can easily detect anomalous SSH components in highly controlled well understood operational environments. Further to Detection uses, HASSH can also be built into the control pipeline as an active component. 
### Bio:
Twin turbo Blueteam Cyberators Ben Reardon (@benreardon) & Adel Karimi. 

## 10.45am - "IoT and the Future of Pentesting" - Jeremy Goldstein
The penetration testing industry and the skills required by pentesters has largely followed the macro trends across the IT industry. From infrastructure and desktop applications, to web apps and mobile apps, and most recently cloud, the significant trends across IT have then naturally influenced what pentesters are targeting and attacking as part of their day job. I've observed over the years that when new technologies gain momentum it is most often through consumer applications. Following an incubation period with consumers, the technology then starts to migrate into traditional businesses and this is most often when pentesters start to get their hands dirty with the technology. I personally experienced this trend with web apps, mobile apps and cloud over the last 15 years. 

This talk will look at the next big trend which is already somewhat finding its way into traditional businesses - the Internet of Things (IoT) and its implications and opportunities for pentesters and the broader security community. Whilst IoT solutions are still largely built upon existing, well understood technologies, there is one big differentiator which will have a significant impact for all of us in the infosec industry... and that's hardware. 

Hardware is so often viewed as a 'secure enough' black box that can be largely ignored and left as is. With the evolving variance of hardware platforms available and implemented in IoT solutions, unfortunately the opposite is painfully true. The hardware component(s) of an IoT solution can be the weakest link and also the hardest one to understand, test and ultimately fix. Hardware comes with its own unique attack surface and threat model which I'll be discussing along with examples demonstrating the impact and outcomes which can be achieved by performing invasive hardware-based testing within standard pentesting engagements. This will incorporate the anatomy of a hardware pentest, including the required testing/attack hardware, skills, considerations and risks all while linking back to real world, tangible examples. 

This talk is aimed at pentesters and security consultants who are busy in the trenches and don’t necessarily get the opportunity to take a step back and consider where the industry is heading. They will gain insight into how hardware affects a solution and how to approach testing it when it is required. The talk is also aimed at the broader infosec industry, including practitioners and leaders who need to understand the implications fast emerging technologies, such as IoT pose to their businesses. They will gain insights into the inner workings of these black boxes and walk away with a much better understanding of the risk profile IoT pose to an organisation and its customers. 
### Bio:
Jeremy (@jg_10) is the Head of Security Testing at TSS and has previously been the team lead of the KPN (Royal Dutch Telecom) REDteam based in Amsterdam, The Netherlands. He has been running technical security teams for a decade and has over 13 years experience in penetration testing and has also spent many of those years performing incident response and threat intelligence. Jeremy enjoys coding, reverse engineering with radare2 and almost anything sufficiently technical... even though he's a team lead. Prior to joining TSS and KPN, Jeremy helped build and run a successful penetration testing, incident response and threat intelligence team at the Australian Taxation Office. 

## 11.45am - "Apathy and Arsenic: a Victorian Era lesson on fighting the surveillance state" - Attacus
What does expensive Victorian era wallpaper have in common with a Cambridge Analytica Facebook quiz? 

Why is the GDPR like a trip to a seaside resort? 

How could a cryptoparty have anything to do with a rare book in a library in Michigan? 
### Bio:
attacus (@attacus_au) is a pen tester, Python wrangler, and recovering historian from Melbourne. She writes and speaks internationally about ethical software, social identities after death, teamwork, and the telegraph. More recently she has researched the domestic use of arsenic in Victorian England, attempted urban camouflage, reverse engineered APIs, wielded the Oxford comma, and knitted a frivolous hat.
She is currently an internet gremlin at Assurance. 

## 12.15pm - Lunch

## 1.15pm - "Wi-Bear: Intelligent Autonomous Wi-Fi Honeypot Detection" - Vivek Ramachandran
Wi-Fi networks are now the pervasive gateways to the Internet. Unfortunately, billions of Wi-Fi clients remain vulnerable to the age old Honeypot attacks where an attacker creates access points which mimic the SSID and settings of an authorized network. Due to easy availability of mature honeypot creation tools, this attack is a slam dunk for even the most novice of Wi-Fi attackers. Enterprise security products have tried but failed to solve this problem with rule and lockdown based approaches. These challenges have compounded with pervasive BYOD and the advent of billions of Wi-Fi enabled home-industrial IoT devices. 

In our talk, we will practically demonstrate how using machine learning techniques we can make Wi-Fi clients “intelligent” so they can autonomously detect Honeypot attacks without needing any central servers. It is important to note that our solution does not need the Wi-Fi radio to be set to monitor mode and uses existing telemetry data provided by the operating system. This makes our technique practically feasible to apply to existing security products without requiring high privileges or underlying low-level operating system changes. We will show that our intelligent Wi-Fi clients can, without exception, detect honeypots created by all popular honeypot tools used in the wild by attacker today. 

### Bio:
Vivek Ramachandran (@securitytube) has been researching Wi-Fi security for over a decade. He discovered the Caffe Latte attack, broke WEP Cloaking, conceptualized enterprise Wi-Fi Backdoors, created Chellam (Wi-Fi Firewall), WiMonitor Enterprise (802.11ac monitoring), Chigula (Wi-Fi traffic analysis via SQL), Deceptacon (IoT Honeypots) and others. He is the author of multiple five star rated books on Wi-Fi security which have together sold over 20,000+ copies worldwide and have been translated to multiple languages. 

Vivek’s work on wireless security (Caffe Latte attack) has been quoted in BBC online, InfoWorld, MacWorld, The Register, IT World Canada and others. He has also spoken/trained at top conferences including BlackHat USA, Europe and Abu Dhabi, Defcon, Brucon, HITB, Hacktivity and others. 

Vivek is also the Founder, CEO of Pentester Academy, AttackDefense.com and Hacker Arsenal. Pentester Academy now trains thousands of customers from government agencies, Fortune 500 companies and smaller enterprises from over 90 countries. It has physical offices in Silicon Valley, USA and Pune, Kolkata in India. 

Nishant Sharma, Ashish Bhangale and Jeswin Mathai from the Pentester Academy team have also contributed to this research. 

## 2.15pm - "windows kernel fuzzing" - gimlu
In recent years, Since the windows kernel security has been highly regarded .the Microsoft enhance the windows kernel security, the kernel issue is fewer and fewer ,and it is more harder for the security researchers to find the kernel issues, So I develop a tools to help the security researchers fuzz the kernel. At first , I will introduce the new method for fuzzing windows kernels. And I will show the fuzzing framework, how it works, will discuss the method for fuzzing windows kernel. The fuzz focused on windows kernel objects, and the relationship with different object, This paper will cover the GDI object and some other windows kernel objects .And I will show some crash cases that I found via the fuzzing framework. At last, I will share some windows crash details, and show a exploit demo that I complete last year. 
### Bio:
Researcher in Tencent Zhanlu Lab (@c0de3). 

## 3.15pm - "Sample 78: Deconstruction Of A Linux Adversarial Toolkit" - Christian Giuffre
Malware analysis presentations are a dime a dozen, unless you want to chat about operating platforms other than Microsoft Windows. Hence, prepare yourself for a walking tour of a real world Linux implant toolkit teardown. Expect to watch the wings of the fly being slowly pulled off only to subsequently experience the excitement of seeing them super glued back together frankenstein style. 

### Bio:
Christian (@int3rrupt) is keenly interested in operating system internals, and understanding the implementation of implants and binary toolkits. Simply, he just likes to know how things work on the inside. During daylight hours Christian is part of the CrowdStrike APT hunt team, fossicking for hands on actors that have infiltrated networks across the globe. In his spare time he also moonlights as an adjunct lecturer at the University of New South Wales. 

## 3.45pm - Break

## 4.00pm - "Source code security audit speed run" - Eldar Marussen
Source code security audit is the process of reviewing source code with the intent of discovering security bugs. This talk will teach the principles of code review, different approaches to code review and some of the tools used for finding vulnerabilities in source code. The talk demonstrates some of the techniques that can be used to quickly identify vulnerabilities and improve static analysis by reducing false positives and adapting static analysis for new things such as templating languages or custom frameworks. The talk also demonstrates practical audits of popular open source applications for different bug classes, including: 
Memory corruption
Logic flaws
Authentication bypass
Injection flaws
and more

### Bio:
Eldar (@Wireghoul) is a lead security researcher with DarkMatter. A highly skilled bug hunter and exploit developer, Eldar was a recipient of the first CVE 10k candidate numbers. He has also developed and delivered training courses on the art of bug hunting. In addition to finding vulnerabilities and writing exploits, he utilises his spare time to contribute to and maintain several open source projects aimed at web application security and penetration testing. 

## 5.00pm - Closing Ceremony

## 6.30pm - Please join us for the Saturday Night Party!!
