﻿using System.Windows;

namespace GeoGen.Studio.Utilities.Messaging
{
    public class MessageThrownEventArgs : RoutedEventArgs
    {
        public Message Message { get; private set; }

        public MessageThrownEventArgs(Message message, RoutedEvent routedEvent)
        {
            this.Message = message;            
            this.RoutedEvent = routedEvent;
        }
    }
}