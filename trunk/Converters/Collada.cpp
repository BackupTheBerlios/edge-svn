
#include <iostream>
#include <string>

#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/dom/DOMDocument.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include "../Render/Node.hpp"
#include "./Collada.hpp"

using namespace std;
using namespace Edge;

XERCES_CPP_NAMESPACE_USE

static void
PrintChildNames(DOMNode* pNode)
{
    DOMNodeList* pChildren = pNode->getChildNodes();
    const XMLCh* pName = NULL;
    DOMNode* pChild;
    short NodeType;
    char* str = NULL;

    for (int i = 0; i < pChildren->getLength(); i++)
    {
        pChild = pChildren->item(i);
        if (DOMNode::ELEMENT_NODE == pChild->getNodeType())
        {
            DOMElement* pElement = dynamic_cast<DOMElement*>(pChild);
            assert(pElement != NULL);
            pName = pElement->getTagName();
            str = XMLString::transcode(pName);
            cout << "Element: " << str << endl;
            XMLString::release(&str);
            
        }
        else if (DOMNode::TEXT_NODE == pChild->getNodeType())
        {
            DOMText* pText = dynamic_cast<DOMText*>(pChild);
            pName = pText->getData();
            str = XMLString::transcode(pName);
            cout << "Text: " << str << endl;
            XMLString::release(&str);
        }
        else if (DOMNode::ENTITY_NODE == pChild->getNodeType())
        {
            DOMEntity* pEntity = dynamic_cast<DOMEntity*>(pChild);
            pName = pEntity->getNodeName();
            str = XMLString::transcode(pName);
            cout << "Entity: " << str << endl;
            XMLString::release(&str);
        }
        PrintChildNames(pChild);
    }
}

Edge::NodePtr
Collada::Import(string Filename)
{
    NodePtr Node;
    XMLPlatformUtils::Initialize();
    XercesDOMParser* Parser = new XercesDOMParser;
    Parser->parse(Filename.c_str());
    DOMDocument* Doc = Parser->getDocument();
    if (!Doc)
    {
        cout << "no document";
        return Node;
    }
    PrintChildNames((DOMNode*)Doc);
    Doc->release();
    XMLPlatformUtils::Terminate();
    return Node;
}

